#pragma once

#include <DirectXTK\Audio.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace Music
{
	class SoundManager
	{
	private:
		std::unique_ptr<DirectX::AudioEngine> m_audioEngine;

		using SEptr = std::unique_ptr<DirectX::SoundEffect>;
		//		std::unordered_map<std::wstring, SEptr> m_soundMap;

		struct SoundData 
		{
			std::unique_ptr<DirectX::SoundEffect> effect;
			std::unique_ptr<DirectX::SoundEffectInstance> effectInstance;
		};

		std::unordered_map<std::wstring, SoundData> m_soundDataMap;

	public:
		void Update();

		void Load(const std::wstring& key, const std::wstring& filePath);

		void Play(const std::wstring& name, bool loop = false);

		void PlayOneShot(const std::wstring& name);

		void Stop(const std::wstring& name);

		void StopAll();

		void Pause(const std::wstring& name);

		void Resume(const std::wstring& name);

		void SetMasterVolume(float volume);

		void Release() { m_soundDataMap.clear(); m_audioEngine.reset(); }

	public:
		static SoundManager& I()
		{
			static SoundManager instance;
			return instance;
		}
	private:
		SoundManager();
		SoundManager(const SoundManager&) = delete;
		SoundManager& operator= (const SoundManager&) = delete;
	};

	//DirectTKの部分を隠蔽したかった
	//特に意味のないクラス

	//SoundEffectのPlay中に終了するとハンドルされない例外が発生する
	// デストラクタで全部Stopする
	class Sound
	{
		DirectX::SoundEffect* m_effect;
		std::unique_ptr<DirectX::SoundEffectInstance> m_sound;
	public:
		void Load(const std::wstring& filePath);
		DirectX::SoundEffectInstance* operator -> ();
		void Play() { m_effect->Play(); }
	};

}