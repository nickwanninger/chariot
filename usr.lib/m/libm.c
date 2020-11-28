#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define MATH

#if 0
#undef MATH
#define MATH                                                                          \
  do {                                                                                \
    if (getpid() != 6) {                                                              \
      fprintf(stderr, "[LIBM pid %d] called math function %s\n", getpid(), __func__); \
    }                                                                                 \
  } while (0)

#endif

#define BAD                                                              \
  do {                                                                   \
    if (1) {                                                             \
      fprintf(stderr, "[LIBM] called bad math function %s\n", __func__); \
    }                                                                    \
  } while (0)


double acos(double x) {
  MATH;
  float negate = (float)(x < 0);
  x = abs(x);
  float ret = -0.0187293;
  ret = ret * x;
  ret = ret + 0.0742610;
  ret = ret * x;
  ret = ret - 0.2121144;
  ret = ret * x;
  ret = ret + 1.5707288;
  ret = ret * sqrt(1.0 - x);
  ret = ret - 2 * negate * ret;
  return negate * 3.14159265358979 + ret;
}
float acosf(float a) { return acos(a); }


double asin(double a) {
  BAD;
  return 0;
}
float asinf(float a) {
  BAD;
  return 0;
}
double atan(double a) {
  BAD;
  return 0;
}
float atanf(float a) {
  BAD;
  return 0;
}
double atan2(double a, double b) {
  BAD;
  return 0;
}
float atan2f(float a, float b) {
  BAD;
  return 0;
}


float cosf(float a) { return cos(a); }
double cosh(double x) { return log(x + sqrt(x * x - 1)); }
float coshf(float x) { return log(x + sqrt(x * x - 1)); }
float sinf(float a) { return sin(a); }

double sinh(double x) {
  MATH;
  double exponentiated = exp(x);
  if (x > 0) return (exponentiated * exponentiated - 1) / 2 / exponentiated;
  return (exponentiated - 1 / exponentiated) / 2;
}
float sinhf(float x) {
  MATH;
  double exponentiated = expf(x);
  if (x > 0) return (exponentiated * exponentiated - 1) / 2 / exponentiated;
  return (exponentiated - 1 / exponentiated) / 2;
}

// bad, slow definition but I don't really think it matters.
double tan(double a) { return sin(a) / cos(a); }
float tanf(float a) { return tan(a); }


double tanh(double a) {
  BAD;
  return 0;
}
float tanhf(float a) {
  BAD;
  return 0;
}
double ceil(double x) {
  MATH;
  int as_int = (int)x;
  if (x == (float)as_int) return as_int;
  if (x < 0) {
    if (as_int == 0) return -0;
    return as_int;
  }
  return as_int + 1;
}


float ceilf(float a) { return ceil(a); }



double floor(double a) {
  MATH;
  if (a >= 0) return (int)a;
  int intvalue = (int)a;
  return ((double)intvalue == a) ? intvalue : intvalue - 1;
}


float floorf(float a) { return floor(a); }
double round(double value) {
  MATH;
  // FIXME: Please fix me. I am naive.
  if (value >= 0.0) return (double)(int)(value + 0.5);
  return (double)(int)(value - 0.5);
}
float roundf(float a) { return round(a); }


union IEEEd2bits {
  double d;
  struct {
    unsigned int manl : 32;
    unsigned int manh : 20;
    unsigned int exp : 11;
    unsigned int sign : 1;
  } bits;
};

int isinf(double d) {
  MATH;
  union IEEEd2bits u;
  u.d = d;
  return (u.bits.exp == 2047 && u.bits.manl == 0 && u.bits.manh == 0);
}

int isnan(double d) { return d != d; }



double fmax(double a, double b) { return a > b ? a : b; }
double fmin(double a, double b) { return a < b ? a : b; }

double trunc(double x) { return (unsigned long)x; }
float truncf(float x) { return (unsigned long)x; }

double cos(double angle) { return sin(angle + M_PI_2); }

double sin(double angle) {
  double res = 0.0;
  asm("fsin" : "=t"(res) : "0"(angle));
  return res;
}

int abs(int a) {
  MATH;
  if (a < 0) {
    return -a;
  }
  return a;
}

double fabs(double a) {
  MATH;
  if (a < 0) {
    return -a;
  }
  return a;
}
float fabsf(float a) {
  MATH;
  if (a < 0) {
    return -a;
  }
  return a;
}
double fmod(double index, double period) {
  MATH;
  return index - trunc(index / period) * period;
}

float fmodf(float index, float period) {
  MATH;
  return index - trunc(index / period) * period;
}


double exp(double a) {
  MATH;
  return pow(2.71828182846, a);
}
float expf(float a) {
  MATH;
  return pow(2.71828182846, a);
}

double frexp(double a, int* exp) {
  MATH;
  BAD;
  return 0;
}
float frexpf(float a, int* exp) {
  MATH;
  BAD;
  return 0;
}
double log(double a) {
  MATH;
  BAD;
  return 0;
}
float logf(float a) {
  MATH;
  BAD;
  return 0;
}
double log10(double a) {
  MATH;
  BAD;
  return 0;
}
float log10f(float a) {
  MATH;
  BAD;
  return 0;
}


double sqrt(double x) {
  MATH;
  double res;
  __asm__("fsqrt" : "=t"(res) : "0"(x));
  return res;
}

float sqrtf(float x) {
  MATH;
  float res;
  __asm__("fsqrt" : "=t"(res) : "0"(x));
  return res;
}
double modf(double a, double* b) {
  MATH;
  BAD;
  return 0;
}
float modff(float a, float* b) {
  MATH;
  BAD;
  return 0;
}
double ldexp(double a, int exp) {
  MATH;
  BAD;
  return 0;
}
float ldexpf(float a, int exp) {
  BAD;
  return 0;
}

double pow(double x, double y) {
  MATH;
  double out;
  asm volatile(
      "fyl2x;"
      "fld %%st;"
      "frndint;"
      "fsub %%st,%%st(1);"
      "fxch;"
      "fchs;"
      "f2xm1;"
      "fld1;"
      "faddp;"
      "fxch;"
      "fld1;"
      "fscale;"
      "fstp %%st(1);"
      "fmulp;"
      : "=t"(out)
      : "0"(x), "u"(y)
      : "st(1)");
  return out;
}
float powf(float x, float y) { return pow(x, y); }

double log2(double a) {
  BAD;
  return 0;
}
float log2f(float a) {
  BAD;
  return 0;
}
long double log2l(long double a) {
  BAD;
  return 0;
}
long double frexpl(long double a, int* b) {
  BAD;
  return 0;
}

double expm1(double x) { return pow(M_E, x) - 1; }

double cbrt(double x) {
  if (x > 0) {
    return pow(x, 1.0 / 3.0);
  }

  return -pow(-x, 1.0 / 3.0);
}

double log1p(double x) { return log(1 + x); }

double acosh(double x) { return log(x + sqrt(x * x - 1)); }

double asinh(double x) { return log(x + sqrt(x * x + 1)); }

double atanh(double x) { return log((1 + x) / (1 - x)) / 2.0; }

double hypot(double x, double y) { return sqrt(x * x + y * y); }

int fesetround(int x) { return 0; }



/*
 * Floats whose exponent is in [1..INFNAN) (of whatever type) are
 * `normal'.  Floats whose exponent is INFNAN are either Inf or NaN.
 * Floats whose exponent is zero are either zero (iff all fraction
 * bits are zero) or subnormal values.
 *
 * A NaN is a `signalling NaN' if its QUIETNAN bit is clear in its
 * high fraction; if the bit is set, it is a `quiet NaN'.
 */
#define	SNG_EXP_INFNAN	255
#define	DBL_EXP_INFNAN	2047
#define	EXT_EXP_INFNAN	32767
struct ieee_double {
  unsigned int dbl_fracl;
  unsigned int dbl_frach : 20;
  unsigned int dbl_exp : 11;
  unsigned int dbl_sign : 1;
};
int fpclassify(double d) {
  struct ieee_double* p = (struct ieee_double*)&d;

  if (p->dbl_exp == 0) {
    if (p->dbl_frach == 0 && p->dbl_fracl == 0)
      return FP_ZERO;
    else
      return FP_SUBNORMAL;
  }

  if (p->dbl_exp == DBL_EXP_INFNAN) {
    if (p->dbl_frach == 0 && p->dbl_fracl == 0)
      return FP_INFINITE;
    else
      return FP_NAN;
  }

  return FP_NORMAL;
}

