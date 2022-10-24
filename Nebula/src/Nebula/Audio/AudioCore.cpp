#include "AudioCore.h"

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <thread>
#include <filesystem>
/*
#include "AL/al.h"
#include "AL/alext.h"
#include "alc/alcmain.h"
#include "alhelpers.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

namespace Nebula {

	static ALCdevice* s_AudioDevice = nullptr;
	static mp3dec_t s_Mp3d;

	static uint8_t* s_AudioScratchBuffer;
	static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially

	static bool s_DebugLog = true;

#define NAUD_LOG(x) std::cout << "[Nebula Audio]  " << x << std::endl

	// Currently supported file formats
	enum class AudioFileFormat
	{
		None = 0,
		Ogg,
		MP3
	};

	static AudioFileFormat GetFileFormat(const std::string& filename)
	{
		std::filesystem::path path = filename;
		std::string extension = path.extension().string();

		if (extension == ".ogg")  return AudioFileFormat::Ogg;
		if (extension == ".mp3")  return AudioFileFormat::MP3;
		
		return AudioFileFormat::None;
	}

	static ALenum GetOpenALFormat(uint32_t channels)
	{
		// Note: sample size is always 2 bytes (16-bits) with
		// both the .mp3 and .ogg decoders that we're using
		switch (channels)
		{
			case 1:  return AL_FORMAT_MONO16;
			case 2:  return AL_FORMAT_STEREO16;
		}
		// assert
		return 0;
	}

	void Audio::LoadAudioSourceOgg(Ref<AudioSource> source)
	{
		FILE* f = fopen(source->m_FilePath.c_str(), "rb");

		OggVorbis_File vf;
		if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
			std::cout << "Could not open ogg stream\n";

		// Useful info
		vorbis_info* vi = ov_info(&vf, -1);
		auto sampleRate = vi->rate;
		auto channels = vi->channels;
		auto alFormat = GetOpenALFormat(channels);

		uint64_t samples = ov_pcm_total(&vf, -1);
		float trackLength = (float)samples / (float)sampleRate; // in seconds
		uint32_t bufferSize = 2 * channels * samples; // 2 bytes per sample (I'm guessing...)

		if (s_DebugLog)
		{
			NAUD_LOG("File Info - " << source->m_FilePath << ":");
			NAUD_LOG("  Channels: " << channels);
			NAUD_LOG("  Sample Rate: " << sampleRate);
			NAUD_LOG("  Expected size: " << bufferSize);
		}

		// TODO: Replace with Nebula::Buffer
		if (s_AudioScratchBufferSize < bufferSize)
		{
			s_AudioScratchBufferSize = bufferSize;
			delete[] s_AudioScratchBuffer;
			s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];
		}

		uint8_t* oggBuffer = s_AudioScratchBuffer;
		uint8_t* bufferPtr = oggBuffer;
		int eof = 0;
		while (!eof)
		{
			int current_section;
			long length = ov_read(&vf, (char*)bufferPtr, 4096, 0, 2, 1, &current_section);
			bufferPtr += length;
			if (length == 0)
			{
				eof = 1;
			}
			else if (length < 0)
			{
				if (length == OV_EBADLINK)
				{
					fprintf(stderr, "Corrupt bitstream section! Exiting.\n");
					exit(1);
				}
			}
		}

		uint32_t size = bufferPtr - oggBuffer;
		// assert bufferSize == size

		if (s_DebugLog)
			NAUD_LOG("  Read " << size << " bytes");

		// Release file
		ov_clear(&vf);
		fclose(f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, oggBuffer, size, sampleRate);

        source->m_BufferHandle = buffer;
        source->m_Loaded = true;
        source->m_TotalDuration = trackLength;
        
		alGenSources(1, &source->m_SourceHandle);
		alSourcei(source->m_SourceHandle, AL_BUFFER, buffer);

		if (alGetError() != AL_NO_ERROR) {
			NAUD_LOG("Failed to setup sound source");
        }
	}

	void Audio::LoadAudioSourceMP3(Ref<AudioSource> source)
	{
		mp3dec_file_info_t info;
		int loadResult = mp3dec_load(&s_Mp3d, source->m_FilePath.c_str(), &info, NULL, NULL);
        ALenum err = alGetError();
        
		uint32_t size = info.samples * sizeof(mp3d_sample_t);


		auto sampleRate = info.hz;
		auto channels = info.channels;
		auto alFormat = GetOpenALFormat(channels);
		float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

        source->m_BufferHandle = buffer;
        source->m_Loaded = true;
        source->m_TotalDuration = lengthSeconds;

		alGenSources(1, &source->m_SourceHandle);
		alSourcei(source->m_SourceHandle, AL_BUFFER, buffer);

		if (s_DebugLog)
		{
			NAUD_LOG("File Info - " << source->m_FilePath << ":");
			NAUD_LOG("  Channels: " << channels);
			NAUD_LOG("  Sample Rate: " << sampleRate);
			NAUD_LOG("  Size: " << size << " bytes");

			auto [mins, secs] = source->GetLengthMinutesAndSeconds();
			NAUD_LOG("  Length: " << mins << "m" << secs << "s");
		}

        err = alGetError();
		if (err != AL_NO_ERROR)
			std::cout << "Failed to setup sound source with error: " << err << std::endl;
	}

	static void PrintAudioDeviceInfo()
	{
		if (s_DebugLog)
		{
			NAUD_LOG("Audio Device Info:");
			NAUD_LOG("  Name: " << s_AudioDevice->DeviceName);
			NAUD_LOG("  Sample Rate: " << s_AudioDevice->Frequency);
			NAUD_LOG("  Max Sources: " << s_AudioDevice->SourcesMax);
			NAUD_LOG("    Mono: " << s_AudioDevice->NumMonoSources);
			NAUD_LOG("    Stereo: " << s_AudioDevice->NumStereoSources);
		}
	}

	void Audio::Init()
	{
		if (InitAL(s_AudioDevice, nullptr, 0) != 0)
			std::cout << "Audio device error!\n";

		PrintAudioDeviceInfo();

		mp3dec_init(&s_Mp3d);

		s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];

		// Init listener
		ALfloat listenerPos[] = { 0.0,0.0,0.0 };
		ALfloat listenerVel[] = { 0.0,0.0,0.0 };
		ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}


    void Audio::SetListenerPos(Vec3f& pos) {
		alListenerfv(AL_POSITION, pos.elements);
    }
    
    void Audio::SetListenerVelocity(Vec3f& vel) {
		alListenerfv(AL_VELOCITY, vel.elements);
    }

    void Audio::SetListenerOrientation(Vec3f& forward, Vec3f& up) {
        ALfloat ori[6];
        ori[0] = forward.X; ori[1] = forward.Y; ori[2] = forward.Z;
        ori[3] = up.X; ori[4] = up.Y; ori[5] = up.Z; 

		alListenerfv(AL_ORIENTATION, ori);
    }

	void Audio::LoadAudioSource(Ref<AudioSource> source)
	{
        source->m_AccessMutex.lock();
		auto format = GetFileFormat(source->m_FilePath);
		switch (format)
		{
			case AudioFileFormat::Ogg:  {
                LoadAudioSourceOgg(source);
                break;
            }
			case AudioFileFormat::MP3: {
                LoadAudioSourceMP3(source);
                break;
            }  
		}
        source->m_AccessMutex.unlock();
	}
	
    // Returns true if started playing
	void Audio::Play(const AudioSource& audioSource)
	{
		// Play the sound until it finishes
        if (audioSource.IsLoaded()) {
            ALint state;
            alGetSourcei(audioSource.m_SourceHandle, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) {
                alSourcePlay(audioSource.m_SourceHandle);
            }
        }
		// TODO: current playback time and playback finished callback
		// eg.
		// ALfloat offset;
		// alGetSourcei(audioSource.m_SourceHandle, AL_SOURCE_STATE, &s_PlayState);
		// ALenum s_PlayState;
		// alGetSourcef(audioSource.m_SourceHandle, AL_SEC_OFFSET, &offset);
	}


    // Returns true if stopped playing
    void Audio::Stop(const AudioSource& audioSource) {
        if (audioSource.IsLoaded()) {
            ALint state;
            alGetSourcei(audioSource.m_SourceHandle, AL_SOURCE_STATE, &state);
            if (state == AL_PLAYING) {
                alSourceStop(audioSource.m_SourceHandle);
            }
        }
    }

	void Audio::SetDebugLogging(bool log)
	{
		s_DebugLog = log;
	}

    
    AudioSource::AudioSource(const std::string& file, bool spatial, bool loop, float gain, float pitch) 
        : m_FilePath(file), m_Spatial(spatial), m_Loop(loop), m_Gain(gain), m_Pitch(pitch) {}

	AudioSource::~AudioSource()
	{
		// TODO: free openal audio source?
        // Invalidate();
	}

    void AudioSource::Play() 
    {
        Audio::Play(*this);
        m_Playing = true; 
    }

    void AudioSource::Stop() {
        Audio::Stop(*this);
        m_Playing = false;
    }

    void AudioSource::Invalidate() {
        if (!IsLoaded()) {
            return;
        }

        Stop();

        m_Loaded = false;
        alDeleteBuffers(1, &m_BufferHandle);
        alDeleteSources(1, &m_SourceHandle);
    }

	void AudioSource::SetPosition(Vec3f pos)
	{
        m_AccessMutex.lock();
		//alSource3f(source, AL_VELOCITY, 0, 0, 0);

		m_Position[0] = pos.X;
		m_Position[1] = pos.Y;
		m_Position[2] = pos.Z;

		alSourcefv(m_SourceHandle, AL_POSITION, m_Position.elements);
        m_AccessMutex.unlock();
	}

	void AudioSource::SetGain(float gain)
	{
        m_AccessMutex.lock();
		m_Gain = gain;

		alSourcef(m_SourceHandle, AL_GAIN, gain);
        m_AccessMutex.unlock();
	}

	void AudioSource::SetPitch(float pitch)
	{
        m_AccessMutex.lock();
		m_Pitch = pitch;

		alSourcef(m_SourceHandle, AL_PITCH, pitch);
        m_AccessMutex.unlock();
	}

	void AudioSource::SetSpatial(bool spatial)
	{
        m_AccessMutex.lock();
		m_Spatial = spatial;

		alSourcei(m_SourceHandle, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
        m_AccessMutex.unlock();
	}

	void AudioSource::SetLoop(bool loop)
	{
        m_AccessMutex.lock();
		m_Loop = loop;

		alSourcei(m_SourceHandle, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        m_AccessMutex.unlock();
	}

	std::pair<uint32_t, uint32_t> AudioSource::GetLengthMinutesAndSeconds() const
	{
		return { (uint32_t)(m_TotalDuration / 60.0f), (uint32_t)m_TotalDuration % 60 };
	}

	Ref<AudioSource> AudioSource::LoadFromFile(const std::string& file, bool spatial)
	{
		return LoadFromFile(file, spatial, false, 1.0f, 1.0f);
	}

    Ref<AudioSource> AudioSource::LoadFromFile(const std::string& file, bool spatial, bool loop, float gain, float pitch)
	{
		Ref<AudioSource> result = CreateRef<AudioSource>(file, spatial, loop, gain, pitch);

		NAUD_LOG("Allocating space for audio file\n");
        std::thread (Audio::LoadAudioSource, result).detach();
        //loadThread.join();

		return result;
	}

}
*/