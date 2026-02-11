#pragma once
#include<memory>// 用于 std::unique_ptr
#include<stdexcept>  // 用于 std::runtime_error
#include<string> // 用于 std::string
#include<unordered_map>// 用于 std::unordered_map
#include<SDL_mixer.h>

namespace engine::resource {

    //管理 SDL_mixer 音效 (Mix_Chunk) 和音乐 (Mix_Music)。

    // 提供音频资源的加载和缓存功能。构造失败时会抛出异常。
      // 仅供 ResourceManager 内部使用。

    class AudioManager final {
        friend class ResourceManager;

    private:
        //Mix_chunk 自定义删除器
        struct SDLMixChunkDeleter {
            void operator()(Mix_Chunk* chunk)const {
                if (chunk)
                {
                    Mix_FreeChunk(chunk);
                }
            }
        };

        // Mix_Music 的自定义删除器
        struct SDLMixMusicDeleter {
            void operator()(Mix_Music* music)
           const {
                if (music)
                {
                    Mix_FreeMusic(music);
                }
            }
        };

        // 音效存储 (文件路径 -> Mix_Chunk)
        std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>>sounds_;

        // 音乐存储 (文件路径 -> Mix_Music)
        std::unordered_map<std::string,
            std::unique_ptr<Mix_Music, SDLMixMusicDeleter>> music_;

    public:
        //构造函数初始化SDL_mixer打开音频设备
        //std::runtime_error

        AudioManager();
        ~AudioManager();

        //依旧放置所有权改变，不需要拷贝构造，移动相关构造
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;


    private:
        Mix_Chunk* loadSound(const std::string& file_path);
        Mix_Chunk* getSound(const std::string& file_path);
        void unloadSound(const std::string& file_path);
        void clearSounds();

          Mix_Music* loadMusic(const std::string& file_path);
          Mix_Music* getMusic(const std::string& file_path);
          void unloadMusic(const std::string& file_path);
          void clearMusic();

          void clearAudio();
    };
}//namespace