#pragma once

namespace impl
{
	template<typename T>
	void get_text_impl(std::wstringstream* dstStr) {/*É_É~Å[*/ }

	template <typename T>
	void  get_text_impl(std::wstringstream* dstStr, const T& t) {
		*dstStr << t;
	}

	template<typename First, typename ...T>
	void get_text_impl(std::wstringstream* dstStr, const First& first, const T& ...arg) {
		get_text_impl(dstStr, first);
		get_text_impl(dstStr, arg...);
	}
}