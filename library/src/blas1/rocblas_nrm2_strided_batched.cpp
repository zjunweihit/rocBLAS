/* ************************************************************************
 * Copyright 2016-2019 Advanced Micro Devices, Inc.
 * ************************************************************************ */
#include "rocblas_nrm2_strided_batched.hpp"
#include "rocblas_reduction_impl.hpp"

namespace
{

    template <typename>
    constexpr char rocblas_nrm2_strided_batched_name[] = "unknown";
    template <>
    constexpr char rocblas_nrm2_strided_batched_name<float>[] = "rocblas_snrm2_strided_batched";
    template <>
    constexpr char rocblas_nrm2_strided_batched_name<double>[] = "rocblas_dnrm2_strided_batched";
    template <>
    constexpr char rocblas_nrm2_strided_batched_name<rocblas_half>[]
        = "rocblas_hnrm2_strided_batched";
    template <>
    constexpr char rocblas_nrm2_strided_batched_name<rocblas_float_complex>[]
        = "rocblas_scnrm2_strided_batched";
    template <>
    constexpr char rocblas_nrm2_strided_batched_name<rocblas_double_complex>[]
        = "rocblas_dznrm2_strided_batched";

    // allocate workspace inside this API
    template <rocblas_int NB, typename Ti, typename To>
    rocblas_status rocblas_nrm2_strided_batched_impl(rocblas_handle handle,
                                                     rocblas_int    n,
                                                     const Ti*      x,
                                                     rocblas_int    incx,
                                                     rocblas_stride stridex,
                                                     rocblas_int    batch_count,
                                                     To*            results)
    {
        static constexpr bool isbatched = true;
        return rocblas_reduction_impl<NB,
                                      isbatched,
                                      rocblas_fetch_nrm2<To>,
                                      rocblas_reduce_sum,
                                      rocblas_finalize_nrm2,
                                      To>(handle,
                                          n,
                                          x,
                                          incx,
                                          stridex,
                                          batch_count,
                                          results,
                                          rocblas_nrm2_strided_batched_name<Ti>,
                                          "nrm2_strided_batched");
    }

}

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */

extern "C" {

#ifdef IMPL
#error IMPL IS ALREADY DEFINED
#endif

#define IMPL(name_, typei_, typeo_)                             \
    rocblas_status name_(rocblas_handle handle,                 \
                         rocblas_int    n,                      \
                         const typei_*  x,                      \
                         rocblas_int    incx,                   \
                         rocblas_stride stridex,                \
                         rocblas_int    batch_count,            \
                         typeo_*        results)                \
    {                                                           \
        constexpr rocblas_int NB = 512;                         \
        return rocblas_nrm2_strided_batched_impl<NB>(           \
            handle, n, x, incx, stridex, batch_count, results); \
    }

IMPL(rocblas_snrm2_strided_batched, float, float);
IMPL(rocblas_dnrm2_strided_batched, double, double);
IMPL(rocblas_scnrm2_strided_batched, rocblas_float_complex, float);
IMPL(rocblas_dznrm2_strided_batched, rocblas_double_complex, double);

#undef IMPL

} // extern "C"
