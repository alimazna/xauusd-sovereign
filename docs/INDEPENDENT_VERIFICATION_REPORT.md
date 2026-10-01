# XAUUSD Sovereign — Independent Verification Report

## Scope
تم فك أرشيف `XAUUSD_Sovereign_Complete(2).rar` وفحص شجرة المشروع والكود المصدر الموجود داخله.

- 526 ملفًا فعليًا داخل المشروع: 371 header + 148 C++ + 7 ملفات جذرية/توثيق نصية، مع مجلدات الأرشيف منفصلة.
- تم بناء `xauusd_foundation` وجميع الاختبارات في وضع `XAUUSD_BUILD_UI=OFF`.
- النتيجة: 33/33 اختبار PASS.
- محاولة البناء بالـUI الافتراضي توقفت عند تنزيل GLFW/ImGui/glad بسبب عدم توفر وصول شبكي إلى GitHub في بيئة الفحص، وليس بسبب compile error في foundation.

## Findings verified as real

### CRITICAL
1. **SL/TP تضيع من RiskProposal إلى Position**
   - `ShadowExecutionEngine.cpp:6-10` لا ينسخ stop_loss/take_profit إلى fill.
   - `SimulatedFill.h:6` لا يحتوي أصلًا على حقول SL/TP.
   - `PositionSimulator.cpp:7` لا يعيّن SL/TP، فيبقيان 0.
   - تم اختبارها end-to-end: Position خرجت بـ SL=0 وTP=0، وأول tick أغلق الصفقة على سعر 0 تقريبًا بخسارة مصطنعة كبيرة.

2. **MarketQuality يصنف spread المستقر الطبيعي كـPOOR**
   - `MarketQualityEngine.cpp:15-18`: ratio قريب من 1.0 ينتهي بـPOOR.
   - اختبار 5 spreads متساوية أعاد القيم: ACCEPTABLE, ACCEPTABLE, ACCEPTABLE, ACCEPTABLE, POOR.

3. **RiskEngine لا يستخدم SymbolSpec في position sizing**
   - `RiskEngine.cpp:10-13`: `volume = risk_amount / stop_distance`.
   - `PositionSizer.cpp:5-10` يستخدم tick_size/tick_value/volume_step/min/max.
   - لا توجد أي references تشغيلية لـPositionSizer من مسار القرار.
   - مثال بافتراض SymbolSpec (`tick_size=.01`, `tick_value=1`): RiskEngine أعطى 10.0 volume بينما PositionSizer أعطى 0.1.

4. **PnL لا يستخدم monetary contract semantics**
   - `PositionSimulator.cpp:20`: `(exit-entry)*volume*mult` فقط.
   - لا يوجد tick_size/tick_value/contract-size/account-currency conversion.
   - مثال tick_size=.01 وtick_value=1: حركة 10.0 على 1 lot تعطي simulator PnL=10، بينما monetary PnL على هذه المواصفات =1000.

5. **PortfolioRiskEngine يحجز positions/exposure ولا توجد آلية release**
   - `PortfolioRiskEngine.cpp:3-8` يزيد counters عند approve.
   - `IPortfolioRiskEngine.h:4` يعرّف approve فقط، بدون release/close/update position.
   - اختبار: أول approve نجح والثاني رُفض عند max_positions=1، ولا يوجد مسار تحرير.
   - كما أن reservation يحصل قبل التأكد أن الـfill نجح في `RuntimeEngine.cpp:69-72`.

6. **AccountState داخل RuntimeEngine ثابت، ولا يوجد API لتحديثه**
   - `RuntimeEngine.h:73-79` يحتوي account_/limits_ كحالة خاصة.
   - `RuntimeEngine.cpp:68` يمرر نفس الحالة إلى RiskEngine دون أي تحديث بعد trades.
   - لا توجد setter/update methods للحساب أو النتائج المحققة.
   - هذا يعني أن equity/balance/daily_pnl/peak_equity في runtime لا تمثل نتيجة الصفقات فعليًا.

7. **ID collisions عبر Multi-Timeframe / parent chain**
   - `SignalEngine.cpp:24`: signal ID يعتمد timestamp + direction + strategy فقط، بلا timeframe.
   - `RiskEngine.cpp:7`: نفس المشكلة proposal ID.
   - `ShadowExecutionEngine.cpp:6`: fill ID لا يعتمد على parent proposal ID.
   - `PositionSimulator.cpp:6`: position ID لا يعتمد على parent fill ID.
   - تم اختبار M15 وH1 بنفس timestamp/direction/strategy وأعطيا نفس signal ID.

8. **Out-of-order / stale state overwrite غير محمي**
   - `FeatureEngine.cpp:18-20`: يضيف bar طالما الزمن الأخير مختلف، بدون شرط `new_time > last_time`.
   - `StructureEngine.cpp:6-9`: نفس النمط.
   - `TimeframeStateStore.cpp:17-20`: overwrites state حسب timeframe حتى لو الـstate الوارد أقدم.
   - اختبار store: حالة 1000 تم استبدالها بحالة أقدم عند 900.

9. **SL/TP geometry غير متحقق منها**
   - `RiskEngine.cpp:9-13`: يستخدم `abs(entry-invalidating)` ولا يفحص اتجاه SL.
   - اختبار LONG مع stop أعلى من entry أعطى proposal approved=true.

10. **RiskLimits غير متحقق من finite/semantic validity، وهناك bypass فعلي**
    - `RiskEngine.cpp` لا يتحقق من limits قبل استخدامها.
    - تم اختبار `min_reward_to_risk=-2`: proposal approved=true وTP صار تحت entry للـLONG.
    - `NaN` في risk parameters يمكن أن يتجاوز بعض المقارنات.

11. **volume reduction لا يعيد حساب risk metadata**
    - `RiskEngine.cpp:16,18`: يخفض volume فقط.
    - `risk_amount` و`risk_fraction` يبقيان بقيم الحجم الأصلي.
    - في اختبار drawdown/DEGRADED أمكن الحصول على volume مخفض مع risk_amount/risk_fraction الأصليين.

12. **close metadata ناقص**
    - `PositionSimulator.cpp:20` لا يكتب `closed_at` ويتجاهل سبب الإغلاق.
    - اختبار close: `closed_at=0` بقي صفرًا.

## Confirmed design/control gaps

13. `BarFinalizer.cpp:11-14` يتجاهل الـTimeframe الذي يعطيه caller؛ و`DataValidator` لا يتحقق من `bar.timeframe`. تم اختبار تمرير caller=H1 مع Bar=M15 فتمت finalization بنجاح.

14. `DataValidator.cpp:11,59,92` يضع `observed_at=Timestamp{}` دائمًا.

15. `DataValidator.cpp:57-87` لا يرفض `event_time > receive_time`؛ اختبار المستقبل الزمني قُبل.

16. `DataValidator::validate_symbol_spec()` لا يرفض NaN؛ اختبار `tick_value=NaN` عاد ACCEPTED.

17. Duplicate bars لا تتحول إلى `DUPLICATE`، بل يتم تجاهلها بصمت في Feature/Structure history. نموذج `DataQualityState` يحتوي DUPLICATE/OUT_OF_ORDER، لكن الـruntime الحالي لا يفعّل هذه الحالات.

18. `CanaryController.cpp:14-24` يقبل decision لطلب غير submitted، ويغير stage؛ تم اختبار الترقية لطلب غير موجود في pending.

19. `ScheduleManager.cpp:11-14` يسمح بأي transition في أي Timestamp بدون فحص schedule windows/timezone/min/max/runtime/research flags؛ تم اختبار transition خارج window ونجح.

20. `GracefulDegradationManager.cpp:18-20` يعيد available إذا كان graph+registry موجودين ولم تسجل capability كـunavailable، حتى لو لم تكن مثبتة كـavailable؛ هذا fail-open.

21. `ResourceGovernor.cpp:15-25` يسمح باستهلاك ResourceType غير الموجود في budget بلا سقف؛ تم اختبار resource غير مهيأ مع `UINT64_MAX` ونجح.

22. `TelegramOrchestrator.cpp:7-27` ينشئ EntityId/Timestamp صفريين لكل status/alert، ما يضعف dedup/audit traceability.

23. `HealthAggregator.cpp:64-95` و`FailureDetector.cpp:27-32` يعيدان snapshots/reports بتواريخ صفرية.

24. `CandidateComparator.cpp:73-84` يترك `compared_at` و`completed_at` بصفر.

25. `RuntimeEngine.cpp:74` يعمل reconciliation مباشرة بعد فتح position فقط؛ ولا توجد دورة post-trade كاملة تحدث account/portfolio state عند الإغلاق.

## Additional finding not highlighted enough in the prior report

**Position.signal_id يأخذ RiskProposal ID وليس Signal ID.**
- `PositionSimulator.cpp:7`: `p.signal_id=f.proposal_id`.
- `SimulatedFill` يحتفظ `proposal_id` فقط.
- النتيجة: الحقل المسمى signal_id في Position يحتوي دلالة risk proposal ID، ما يكسر parent identity semantics حتى قبل مشكلة collision.

## Reconciliation gap

`ReconciliationEngine.cpp:4-9` يتحقق من direction + entry + stop فقط، ولا يتحقق من take_profit، volume، fill identity، أو closed state. تم اختبار Position لديها TP مختلف جدًا عن signal وبقيت `matches=true`.

## Test coverage finding

الـ33 اختبار الحالية كلها تمر، لكن `tests/integration/test_full_pipeline.cpp` لا يثبت وجود trade فعلي. إعادة نفس السيناريو المضمن في الاختبار أعادت:
- signals = 0
- fills = 0
- completed decisions = 0
- ledger entries = 0

بالتالي نجاح الاختبار لا يختبر فعليًا مسار `Risk -> Portfolio -> Fill -> Position -> Reconciliation`.

## Prototype status that should not be mislabeled as bugs

- `ProbabilityEngine` صراحةً غير معاير.
- `MT5Bridge` inert/deferred، ولا يوجد live order execution.
- `Sandbox` lifecycle simulator وليس isolation حقيقيًا.
- `ValidationRunner` deterministic gate checks وليس Monte-Carlo/OOS backtest حقيقي.

## Final assessment

النتيجة المستقلة تؤكد جوهر التقرير السابق، بل وتكشف عدة نقاط إضافية مهمة. المشروع **يترجم ويجتاز الاختبارات الحالية، لكنه غير متسق ماليًا في مسار التنفيذ/المحاكاة** بسبب SL/TP، sizing، PnL، state updates، reservations، identity، وdata ordering.

لذلك لا يصح اعتبار `33/33 PASS` دليلًا على جاهزية shadow trading المالي؛ هو دليل على أن الـinterfaces والـhappy paths الحالية تمر.
