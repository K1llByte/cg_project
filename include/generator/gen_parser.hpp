#ifndef GEN_PARSER_HPP
#define GEN_PARSER_HPP

#include "common/vertex.hpp"
#include <vector>

// TODO: Add this two to namespace 'cg'

cg::vnt_vectors gen_by_strviews(const std::vector<std::string_view>& cmd);

cg::vnt_vectors gen_by_command(const std::string& cmd);

#endif // GEN_PARSER_HPP