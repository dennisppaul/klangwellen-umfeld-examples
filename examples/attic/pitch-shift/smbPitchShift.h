/****************************************************************************
*
* NAME: smbPitchShift.cpp
* VERSION: 1.2
* HOME URL: http://www.dspdimension.com
* KNOWN BUGS: none
*
* SYNOPSIS: Routine for doing pitch shifting while maintaining
* duration using the Short Time Fourier Transform.
*
* DESCRIPTION: The routine takes a pitchShift factor value which is between 0.5
* (one octave down) and 2. (one octave up). A value of exactly 1 does not change
* the pitch. numSampsToProcess tells the routine how many samples in indata[0...
* numSampsToProcess-1] should be pitch shifted and moved to outdata[0 ...
* numSampsToProcess-1]. The two buffers can be identical (ie. it can process the
* data in-place). fftFrameSize defines the FFT frame size used for the
* processing. Typical values are 1024, 2048 and 4096. It may be any value <=
* MAX_FRAME_LENGTH but it MUST be a power of 2. osamp is the STFT
* oversampling factor which also determines the overlap between adjacent STFT
* frames. It should at least be 4 for moderate scaling ratios. A value of 32 is
* recommended for best quality. sampleRate takes the sample rate for the signal
* in unit Hz, ie. 44100 for 44.1 kHz audio. The data passed to the routine in
* indata[] should be in the range [-1.0, 1.0], which is also the output range
* for the data, make sure you scale the data accordingly (for 16bit signed integers
* you would have to divide (and multiply) by 32768).
*
* COPYRIGHT 1999-2009 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
*
* 						The Wide Open License (WOL)
*
* Permission to use, copy, modify, distribute and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice and this license appear in all source copies.
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
* ANY KIND. See http://www.dspguru.com/wol.htm for more information.
*
*****************************************************************************/

#pragma once

/**
 * Purpose: doing pitch shifting while maintaining duration using the Short Time Fourier Transform.
 * Author: (c)1999-2009 Stephan M. Bernsee <smb [AT] dspdimension [DOT] com>
 *
 * @param pitchShift factor value which is between 0.5 (one octave down) and 2.0 (one octave up). A value of exactly
 * 1 does not change the pitch.
 * @param numSampsToProcess numSampsToProcess tells the routine how many samples in indata[0 ... numSampsToProcess-1]
 * should be pitch shifted and moved to outdata[0 ... numSampsToProcess-1]. The two buffers can be identical (ie. it
 * can process the data in-place).
 * @param fftFrameSize defines the FFT frame size used for the processing. Typical values are 1024, 2048 and 4096. It
 * may be any value <= MAX_FRAME_LENGTH, but it MUST be a power of 2
 * @param osamp is the STFT oversampling factor which also determines the overlap between adjacent STFT frames. It
 * should at least be 4 for moderate scaling ratios. A value of 32 is recommended for best quality.
 * @param sampleRate takes the sample rate for the signal in unit Hz, ie. 44100 for 44.1 kHz audio.
 * @param indata The data passed to the routine in indata[] should be in the range [-1.0, 1.0], which is also the
 * output range for the data, make sure you scale the data accordingly (for 16bit signed integers you would have to
 * divide (and multiply) by 32768).
 * @param outdata
 */
void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, const float* indata, float* outdata);
