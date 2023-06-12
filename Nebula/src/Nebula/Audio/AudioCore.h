#pragma once

#include <iostream>
#include <string>
#include <Math/Math.h>
#include <Core/Ref.h>

#include <thread>
#include <mutex>

/*
namespace Nebula {

	class AudioSource
	{
	public:

		AudioSource() = default;
        AudioSource(const std::string& file, bool spatial, bool loop, float gain, float pitch);
		~AudioSource();

		bool IsLoaded() const { return m_Loaded; }
        std::string& GetFilePath() { return m_FilePath; }

		Vec3f GetPosition() { return m_Position; }
		float GetGain()     { return m_Gain; }
		float GetPitch()    { return m_Pitch; }
		bool GetSpatial()   { return m_Spatial; }
		bool GetLoop()      { return m_Loop; }

		void SetPosition(Vec3f pos);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetSpatial(bool spatial);
		void SetLoop(bool loop);

        void Play();
        void Stop();

        bool IsPlaying() { return m_Playing; }

        void Invalidate();

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;

		static Ref<AudioSource> LoadFromFile(const std::string& file, bool spatial = false);
		static Ref<AudioSource> LoadFromFile(const std::string& file, bool spatial, bool loop, float gain, float pitch);
	private:

		uint32_t m_BufferHandle = 0;
		uint32_t m_SourceHandle = 0;
		bool m_Loaded = false;
		bool m_Spatial = false;
        bool m_Playing = false;

		float m_TotalDuration = 0; // in seconds
		
		// Attributes
		Vec3f m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		bool m_Loop = false;


        std::string m_FilePath = std::string();

        std::mutex m_AccessMutex = std::mutex();

		friend class Audio;
	};

	class Audio
	{
	public:
		static void Init();

		static void LoadAudioSource(Ref<AudioSource> source);
		static void Play(const AudioSource& source);
        static void Stop(const AudioSource& source);

        static void SetListenerPos(Vec3f& pos);
        static void SetListenerVelocity(Vec3f& pos);
        static void SetListenerOrientation(Vec3f& forward, Vec3f& up);

		// TODO: temporary whilst Nebula Audio is in early dev
		static void SetDebugLogging(bool log);
	private:
		static void LoadAudioSourceOgg(Ref<AudioSource> source);
		static void LoadAudioSourceMP3(Ref<AudioSource> source);
	};

}
*/