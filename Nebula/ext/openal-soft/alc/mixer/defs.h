#ifndef MIXER_DEFS_H
#define MIXER_DEFS_H

#include <array>

#include "alspan.h"
#include "core/bufferline.h"

struct DirectHrtfState;
struct HrtfFilter;
union InterpState;
struct MixHrtfFilter;

using uint = unsigned int;
using float2 = std::array<float,2>;


constexpr int MixerFracBits{12};
constexpr int MixerFracOne{1 << MixerFracBits};
constexpr int MixerFracMask{MixerFracOne - 1};

/* Maximum number of samples to pad on the ends of a buffer for resampling.
 * Note that the padding is symmetric (half at the beginning and half at the
 * end)!
 */
constexpr int MaxResamplerPadding{48};


template<typename TypeTag, typename InstTag>
const float *Resample_(const InterpState *state, const float *RESTRICT src, uint frac,
    uint increment, const al::span<float> dst);

template<typename InstTag>
void Mix_(const al::span<const float> InSamples, const al::span<FloatBufferLine> OutBuffer,
    float *CurrentGains, const float *TargetGains, const size_t Counter, const size_t OutPos);

template<typename InstTag>
void MixHrtf_(const float *InSamples, float2 *AccumSamples, const uint IrSize,
    const MixHrtfFilter *hrtfparams, const size_t BufferSize);
template<typename InstTag>
void MixHrtfBlend_(const float *InSamples, float2 *AccumSamples, const uint IrSize,
    const HrtfFilter *oldparams, const MixHrtfFilter *newparams, const size_t BufferSize);
template<typename InstTag>
void MixDirectHrtf_(FloatBufferLine &LeftOut, FloatBufferLine &RightOut,
    const al::span<const FloatBufferLine> InSamples, float2 *AccumSamples, DirectHrtfState *State,
    const size_t BufferSize);

/* Vectorized resampler helpers */
inline void InitPosArrays(uint frac, uint increment, uint *frac_arr, uint *pos_arr, size_t size)
{
    pos_arr[0] = 0;
    frac_arr[0] = frac;
    for(size_t i{1};i < size;i++)
    {
        const uint frac_tmp{frac_arr[i-1] + increment};
        pos_arr[i] = pos_arr[i-1] + (frac_tmp>>MixerFracBits);
        frac_arr[i] = frac_tmp&MixerFracMask;
    }
}

#endif /* MIXER_DEFS_H */
