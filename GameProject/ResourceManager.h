#pragma once
#include "Utilities.h"

template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string& l_path)
	{
		loadPaths(l_path);
	}
	virtual ~ResourceManager()
	{
		purgeResources();
	}
	T* getResource(const std::string& l_id)
	{
		auto res = find(l_id);
		return(res ? res->first : nullptr);
	}
	std::string getPath(const std::string& l_id)
	{
		auto path = m_paths.find(l_id);
		return(path != m_paths.end() ? path->second : "");
	}
	bool requireResource(const std::string& l_id)
	{
		auto res = find(l_id);
		if (res)
		{
			++res->second;
			return true;
		}
		auto path = m_paths.find(l_id);
		if (path == m_paths.end()) return false;
		T* resource = load(path->second);
		if (!resource) return false;
		m_resources.emplace(l_id, std::make_pair(resource, 1));
		return true;
	}
	bool releaseResource(const std::string& l_id)
	{
		auto res = find(l_id);
		if (!res) return false;
		--res->second;
		if (!res->second)
		{
			unload(l_id);
		}
		return true;
	}

	void purgeResources()
	{
		while (m_resources.begin() != m_resources.end())
		{
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	T* load(const std::string& l_path)
	{
		return static_cast<Derived*>(this)->load(l_path);
	}

private:
	std::unordered_map<std::string, std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;

	std::pair<T*, unsigned int>* find(const std::string& l_id)
	{
		auto itr = m_resources.find(l_id);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}
	bool unload(const std::string& l_id)
	{
		auto itr = m_resources.find(l_id);
		if (itr == m_resources.end()) return false;
		delete itr->second.first;
		m_resources.erase(itr);
		return true;
	}
	void loadPaths(const std::string& l_pathsFile)
	{
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory() + l_pathsFile);
		if (paths.is_open())
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keyStream(line);
				std::string pathName;
				std::string path;
				keyStream >> pathName;
				keyStream >> path;
				m_paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}
		std::cerr << "! Failed loading the path file:" << l_pathsFile << "\n";
	}
};

