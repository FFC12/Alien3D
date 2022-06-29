#ifndef ALIEN3D_SHADER_PARSER_H
#define ALIEN3D_SHADER_PARSER_H

#include <map>
#include <string>
#include <utility>

namespace ShaderParser {
    // TODO: Potential bugs here..
    static void ParseMustache(std::string &buffer, const std::string &lookFor, const std::string &replacement) {
        size_t i = 0;

        if (buffer.empty()) return;

        std::map<std::string, std::pair<size_t, size_t>> mustaches;
        while (i < buffer.size() - 1) {
            if (buffer[i] == '{') {
                if (i < buffer.size() - 1) {
                    if (buffer[i + 1] == '{') {
                        size_t beginPos = i;
                        i += 1; // advance '{'
                        i += 1;

                        while (buffer[i] == ' ' && i < buffer.size() - 1) {
                            i += 1;
                        }

                        // {{ mustache }}
                        std::string name;
                        while ((std::isalpha(buffer[i]) || buffer[i] == '_' || buffer[i] == '.') &&
                               (i < buffer.size() - 1)) {
                            name += buffer[i];
                            i += 1;
                        }

                        while (buffer[i] == ' ' && i < buffer.size() - 1) {
                            i += 1;
                        }

                        if (buffer[i] == '}') {
                            if (i < buffer.size() - 1) {
                                if (buffer[i + 1] == '}') {
                                    i += 1;
                                    i += 1;

                                    size_t endPos = i - 1;
                                    if (name == lookFor) {
                                        mustaches[replacement] = {beginPos, endPos};

                                    }
                                } else {
                                    abort();
                                }
                            }
                        } else {
                            abort();
                        }
                    }
                }
            }

            i += 1;
        }

        // {{ mustache }}
        // {{ alien3d  }}
        size_t lastPayloadSize = 0;
        for (auto &mustache: mustaches) {
            buffer.erase(mustache.second.first + lastPayloadSize, (mustache.second.second - mustache.second.first + 1));
            buffer.insert(mustache.second.first + lastPayloadSize, mustache.first);
            lastPayloadSize = (mustache.second.second - mustache.second.first + 1);
        }
    }
}

#endif