// // #ifndef UTILS_HPP
// // # define UTILS_HPP

// #include <string>
// #include <vector>

// bool	allowedCharacter(char c) {
// 	if ((c >= 65 && c <= 90) /*'A'-'Z'*/ || (c >= 97 && c <= 123) /*'a'-'z'*/ || (c >= 48 && c <= 57) /*'0'-'9'*/ || (c == '-' || c == '_'))
// 		return true;
// 	return false;
// }

// bool	validateString(const std::string &string)
// {
// 	for (std::vector<const char>::iterator it = string.begin(); it != string.end(); ++it)
// 		if (!allowedCharacter(*it))
// 			return false;
// 	return true;
// }

// // #endif