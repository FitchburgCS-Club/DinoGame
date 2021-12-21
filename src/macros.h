#ifndef MACROS_H
#define MACROS_H
// Defines an error printing macro that is crossplatform
// because Windows doesn't have a stderr stream, so you have to
// use standard printf
#ifdef __unix__
#define eprintf(...) fprintf(stderr, __VA_ARGS__);
#else
#define eprintf(...) printf(__VA_ARGS__);
#endif
#endif
