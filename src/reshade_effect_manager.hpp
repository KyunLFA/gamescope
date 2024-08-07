#pragma once

#include "rendervulkan.hpp"
#include <optional>

namespace reshadefx
{
    struct effect_module;
}

class ReshadeUniform;

struct ReshadeCombinedImageSampler
{
    VkSampler sampler;
    gamescope::Rc<CVulkanTexture> texture;
};

struct ReshadeEffectKey
{
	std::string path;

	uint32_t bufferWidth;
	uint32_t bufferHeight;
	GamescopeAppTextureColorspace bufferColorSpace;
    VkFormat bufferFormat;

	uint32_t techniqueIdx;

    bool operator==(const ReshadeEffectKey& other) const = default;
    bool operator!=(const ReshadeEffectKey& other) const = default;
};

enum ReshadeDescriptorSets
{
    GAMESCOPE_RESHADE_DESCRIPTOR_SET_UBO = 0,
    GAMESCOPE_RESHADE_DESCRIPTOR_SET_SAMPLED_IMAGES,
    GAMESCOPE_RESHADE_DESCRIPTOR_SET_STORAGE_IMAGES,

    GAMESCOPE_RESHADE_DESCRIPTOR_SET_COUNT,
};

class ReshadeEffectPipeline
{
public:
    ReshadeEffectPipeline();
    ~ReshadeEffectPipeline();

    bool init(CVulkanDevice *device, const ReshadeEffectKey &key);
    void update();
    uint64_t execute(gamescope::Rc<CVulkanTexture> inImage, gamescope::Rc<CVulkanTexture> *outImage);

    const ReshadeEffectKey& key() const { return m_key; }
    reshadefx::effect_module *effect_module() { return m_module.get(); }

    gamescope::Rc<CVulkanTexture> findTexture(std::string_view name);

private:
    ReshadeEffectKey m_key;
    CVulkanDevice *m_device;

	std::unique_ptr<reshadefx::effect_module> m_module;
    std::vector<VkPipeline> m_pipelines;
    std::vector<gamescope::OwningRc<CVulkanTexture>> m_textures;
    gamescope::OwningRc<CVulkanTexture> m_rt;
    std::vector<ReshadeCombinedImageSampler> m_samplers;
    std::vector<std::shared_ptr<ReshadeUniform>> m_uniforms;

    std::optional<CVulkanCmdBuffer> m_cmdBuffer = std::nullopt;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
    void* m_mappedPtr = nullptr;

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

    VkDescriptorSetLayout m_descriptorSetLayouts[GAMESCOPE_RESHADE_DESCRIPTOR_SET_COUNT] = {};
    VkDescriptorSet m_descriptorSets[GAMESCOPE_RESHADE_DESCRIPTOR_SET_COUNT] = {};
};

class ReshadeEffectManager
{
public:
    ReshadeEffectManager();

    void init(CVulkanDevice *device);
    void clear();
    ReshadeEffectPipeline* pipeline(const ReshadeEffectKey &key);

private:
    ReshadeEffectKey m_lastKey{};
    std::unique_ptr<ReshadeEffectPipeline> m_lastPipeline;
    CVulkanDevice *m_device;
};

extern ReshadeEffectManager g_reshadeManager;
