#include "Sound.h"

#pragma comment(lib, "DirectXTKAudioWin8.lib")

namespace Music
{
	SoundManager::SoundManager()
	{
		m_audioEngine = std::make_unique<DirectX::AudioEngine>();
	}

	void SoundManager::Update()
	{
		if (!m_audioEngine->Update())
		{
			// No audio device is active
			if (m_audioEngine->IsCriticalError())
			{
				throw std::exception("オーディオデバイスがない");
			}
		}
	}

	void SoundManager::Load(const std::wstring& key, const std::wstring& filePath)
	{
		//		if (m_soundMap.find(key) != m_soundMap.end())
		//			throw std::runtime_error("既存のキーです");

		SoundData data;
		data.effect = std::make_unique<DirectX::SoundEffect>(m_audioEngine.get(), filePath.c_str());
		data.effectInstance = data.effect->CreateInstance();
		m_soundDataMap[key] = std::move(data);
	}

	void SoundManager::Play(const std::wstring& name, bool loop)
	{
		m_soundDataMap.at(name).effectInstance->Play(loop);
	}

	void SoundManager::PlayOneShot(const std::wstring& name)
	{
		m_soundDataMap.at(name).effect->Play();
	}

	void SoundManager::Stop(const std::wstring& name)
	{
		m_soundDataMap.at(name).effectInstance->Stop();
	}

	void SoundManager::StopAll()
	{
		for (auto&& sound : m_soundDataMap)
		{
			sound.second.effectInstance->Stop(true);
		}
	}

	void SoundManager::Pause(const std::wstring& name)
	{
		m_soundDataMap.at(name).effectInstance->Pause();
	}

	void SoundManager::Resume(const std::wstring& name)
	{
		m_soundDataMap.at(name).effectInstance->Resume();
	}

	void SoundManager::SetMasterVolume(float volume)
	{
		m_audioEngine->SetMasterVolume(volume);
	}

	void Sound::Load(const std::wstring& filePath)
	{
		//		using SEptr = std::unique_ptr<DirectX::SoundEffect>;
		//		static std::unordered_map<std::wstring, SEptr> s_soundMap;

		//		if (s_soundMap.find(filePath) == s_soundMap.end())
		//			s_soundMap.emplace(filePath,
		//				std::make_unique<DirectX::SoundEffect>(s_audioEngine.get(), filePath.c_str()));

		//		m_effect = s_soundMap.at(filePath).get();
		//		m_sound = m_effect->CreateInstance();
	}


	DirectX::SoundEffectInstance* Sound::operator -> ()
	{
		return m_sound.get();
	}
}