# Differences between GNU, C99 and C++ `inline`

|No. |`inline` type           |Out-of-line definition disposition |Out-of-line definition linkage |
|:---|:-----------------------|:----------------------------------|:------------------------------|
|1.1 |GNU plain `inline`  [1] |Not emitted                        |N/A                            |
|1.2 |GNU `static inline` [1] |Emitted only when inlining fails   |internal                       |
|1.3 |GNU `extern inline` [1] |Not emitted                        |N/A                            |
|2.1 |C99 plain `inline`  [1] |Not emitted                        |N/A                            |
|2.2 |C99 `static inline` [1] |Emitted only when inlining fails   |internal                       |
|2.3 |C99 `extern inline` [1] |Always emitted                     |external, strong               |
|3.1 |C++ plain `inline`  [1] |Emitted only when inlining fails   |external, weak [2]             |
|3.2 |C++ `static inline` [1] |Emitted only when inlining fails   |internal                       |
|3.3 |C++ `extern inline` [1] |Emitted only when inlining fails   |external, weak [2]             |

1. If a function is declared the first time with `inline` and a second time with `extern` then it is considered `extern inline`.
2. On Windows `.weak` symbols may be emitted as `.linkonce` ones instead.
