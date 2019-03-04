#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include "../base/Math.h"


namespace tama
{
	template<class Data> class SceneManager;

	template<class Data>
	class SceneBase
	{
		friend class SceneManager<Data>;
	public:
		enum class State : uint16_t
		{
			DoNothing = 1 << 0,
			Draw = 1 << 1,
			Update = 1 << 2,
			Active = Draw | Update,
		};

	protected:
		State m_state = State::DoNothing;
		uint16_t m_priority = 0;

		std::shared_ptr<Data> m_data;
		SceneManager<Data>* m_pManager = nullptr;

	public:
		void Set(SceneManager<Data>* manager, const std::shared_ptr<Data>& data)
		{
			m_pManager = manager;
			m_data = data;
		}

		virtual void Init() {}
		virtual bool Update() { return true; }
		virtual void Draw() const{}
		virtual ~SceneBase() = default;
		State GetState() const { return m_state; }
		uint16_t GetPriority() const { return m_priority; }
		bool isUpdatable() const { return ((static_cast<uint16_t>(m_state) & static_cast<uint16_t>(State::Update)) == static_cast<uint16_t>(State::Update)); }
		bool isDrawable() const { return ((static_cast<uint16_t>(m_state)& static_cast<uint16_t>(State::Draw)) == static_cast<uint16_t>(State::Draw)); }

		bool operator < (const SceneBase<Data>& right)
		{
			return m_priority < right.m_priority;
		}
	};

	template<class Data>
	class SceneManager
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<SceneBase<Data>>> m_sceneContainer;
		std::shared_ptr<Data> m_data;

	public:
		using Scene = SceneBase < Data > ;

		SceneManager() :
			m_data(std::make_shared<Data>())
		{}

		SceneManager(Data&& initialData) :
			m_data(std::make_shared<Data>(std::move(initialData)))
		{}

		bool Update()
		{
			std::vector<std::shared_ptr<Scene>> updatable;

			for (auto& info : m_sceneContainer)
			{
				if (info.second->isUpdatable())
					updatable.emplace_back(info.second);
			}

			for (auto&& scene : updatable){
				if (!scene->Update())
					return false;
			}
			return true;
		}

		void Draw() const
		{
			std::vector<std::shared_ptr<Scene>> drawable;

			for (auto&& info : m_sceneContainer)
			{
				if (info.second->isDrawable())
				{
					drawable.emplace_back(info.second);
				}
			}

			std::sort(drawable.begin(), drawable.end(), [](const std::shared_ptr<Scene>& left, const std::shared_ptr<Scene>& right)
			{
				return (*left < *right);  
			});

			for (const auto& scene : drawable)
			{
				scene->Draw();
			}		
		}

		/**@brief 新しいシーンを読み込む
		*@template Scene 読み込むシーンクラス
		*@param[in] state 読み込むシーンの状態
		*/
		template<class _Scene>
		void LoadScene(typename Scene::State state = Scene::State::Active, bool doInit = true)
		{
			std::string key(typeid(_Scene).name());
			key.erase(0, 6); //そのままだとkey = class 名前 になるので前から6文字消して名前だけをキーにする

			if (m_sceneContainer.find(key) != m_sceneContainer.end())
			{
				auto scene = m_sceneContainer.at(key);
				scene->m_state = state;
				if(doInit)
					scene->Init();
				
				return;
			}

			auto newScene = std::make_shared<_Scene>();
			newScene->Set(this, m_data);
			if(doInit)
				newScene->Init();
			newScene->m_state = state;
			m_sceneContainer.emplace(key, newScene);
		}

		/**@brief シーンを削除する 
		*@param className 登録しているクラス名
		*@return 成功->true, 見つからない->false
		*/
		bool EraseScene(const std::string& className)
		{
			if (m_sceneContainer.find(className) == m_sceneContainer.end())
			{
				return false;
			}

//			m_sceneContainer.at(className) = nullptr;
			m_sceneContainer.erase(className);

			return true;
		}

		std::shared_ptr<Scene> GetScene(const std::string& className)
		{
			return m_sceneContainer.at(className);
		}

		void SetSceneActive(const std::string& className)
		{
			m_sceneContainer.at(className)->m_state = Scene::State::Active;
		}
	};
}

