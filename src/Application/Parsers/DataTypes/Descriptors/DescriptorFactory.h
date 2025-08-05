#ifndef _DESCRIPTOR_FACTORY_H
#define _DESCRIPTOR_FACTORY_H

#include <functional>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>

#include "src/Application/Parsers/DataTypes/Descriptors/DescriptorTemplatization.h"
#include "src/Application/Parsers/DataTypes/Descriptors/Descriptor.h"

namespace Descriptors {
    
    using FactoryFunction = std::function<std::unique_ptr<Descriptor>(const std::uint8_t length, const std::uint8_t* start)>;

    class DescriptorFactory {
    public:
        static DescriptorFactory& instance() {
            static DescriptorFactory instance;
            return instance;
        }

        inline bool registerFactory(std::uint8_t tag, FactoryFunction factory_function) {
            return factories_.emplace(tag, std::move(factory_function)).second;
        }

        inline std::unique_ptr<Descriptor> make(const std::uint8_t tag, const std::uint8_t length, const std::uint8_t* start) const {
            auto it = this->factories_.find(tag);
            if (it == factories_.end()) {
                throw std::runtime_error(
                    "[DecsriptorFactory] Failed to dynamically create a descriptor: Descriptor with tag " 
                    + std::to_string(tag) + " did not register."
                );
            }

            return it->second(length, start);
        }


    private:
        std::unordered_map<std::uint8_t, FactoryFunction> factories_;

        DescriptorFactory() = default;
        DescriptorFactory(const DescriptorFactory&) = delete;
        DescriptorFactory& operator=(const DescriptorFactory&) = delete;
    };
} // namespace Descriptors


#endif // _DESCRIPTOR_FACTORY_H