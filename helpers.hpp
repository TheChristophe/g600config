#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>

#define STR_(x) #x
#define STR(x) STR_(x)

#define Try(call)                                                                                                                                    \
	do {                                                                                                                                             \
		result = call;                                                                                                                               \
		if (result != 0) {                                                                                                                           \
			std::cout << __FILE__ << ":" << __LINE__ << " " << STR(call) << ": " << result << " (" << libusb_error_name(result) << ")" << std::endl; \
		}                                                                                                                                            \
	} while (0)

#define TryElseReturn(call, value) \
	do {                           \
		Try(call);                 \
		if (result < 0) {          \
			return value;          \
		}                          \
	} while (0)

#define TryElseGoto(call, label) \
	do {                         \
		Try(call);               \
		if (result < 0) {        \
			goto label;          \
		}                        \
	} while (0)

extern int result;

#endif
