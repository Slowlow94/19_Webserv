#include "ConfigParser.hpp"

bool ConfigParser::endsWith(const std::string path, const std::string extension) {
	if (extension.size() > path.size())
		return (false);
	return (path.substr(path.size() - extension.size()) == extension);
}

std::vector<std::string> ConfigParser::split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	
	for (std::size_t i = 0; i < str.size(); ++i) {
		if (str[i] == delimiter) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else {
			token += str[i];
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return (tokens);
}

std::string ConfigParser::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
		return ("");
	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return (str.substr(start, end - start + 1));
}

bool ConfigParser::is_only_whitespace(const std::string& str) {
	for (size_t i = 0; i < str.size(); ++i) {
		if (!std::isspace(str[i]))
			return (false);
	}
	return (true);
}

bool ConfigParser::is_token_valid_multiple(const std::string& line, const std::vector <std::string> tokens) {
	for (size_t i = 0; i < tokens.size(); ++i) {
		if (line.find(tokens[i]) != std::string::npos)
			return (true);
	}
	return (false);
}

bool ConfigParser::is_token_valid(const std::string& line, const std::string token) {
	return (line == token);
}

void ConfigParser::initializeVector(std::vector<std::string>& vec, std::string tokens) {
    std::vector <std::string> vec_tokens = split(tokens, ' ');
	vec = vec_tokens;
}

bool ConfigParser::isStringDigit(std::string args) {
	if (args.empty())
		return (false);
	for (std::size_t i = 0; i < args.size(); i++) {
		if (!std::isdigit(args[i]))
			return (false);
	}
	return (true);
}

bool ConfigParser::isValidServerName(std::string name) {
	bool last_dot_occurence;

	last_dot_occurence = false;
	if (name.empty())
		return (false);
	if (!std::isalpha(name[0]))
		return (false);
	for (std::size_t i = 0; i < name.size(); i++) {
		char c = name[i];
		if (std::isalnum(c) || c == '-' || c == '.') {
			if ((i == 0 || i == name.size() - 1) && (c == '.' || c == '-')) 
				return (false);
			if (c == '.') {
				if (last_dot_occurence) 
					return (false);
				last_dot_occurence = true;
			} else
				last_dot_occurence = false;
		} else
			return (false);
	}
	return (true);
}

bool ConfigParser::isValidURL(const std::string& url) {
	const std::string http  = "http://";
	const std::string https = "https://";
	std::size_t pos = 0;
	std::size_t slash_pos;
	std::string domain;

	if (url.compare(0, http.size(), http) == 0) {
		pos = http.size();
	} else if (url.compare(0, https.size(), https) == 0) {
		pos = https.size();
	} else {
		return (false);
	}

    slash_pos = url.find('/', pos);
    domain = (slash_pos == std::string::npos) ? url.substr(pos) : url.substr(pos, slash_pos - pos);

    if (!isValidServerName(domain))
        return (false);

    if (slash_pos != std::string::npos) {
        for (std::size_t i = slash_pos; i < url.size(); i++) {
            char c = url[i];
            if (!(std::isalnum(c) || c == '/' || c == '.' || c == '-' || c == '_' || c == '?' || c == '&' || c == '=' || c == '%')) {
                return (false);
            }
        }
    }
    
    return (true);
}

std::string ConfigParser::returnSecondArgs(std::string args) {
	std::string trimmed_line;
	std::string second_args;
	std::size_t i;

	trimmed_line = trim(args);
	i = trimmed_line.find(' ');
	if (i == std::string::npos)
		return ("");
	second_args = trimmed_line.substr(i + 1, std::string::npos);
	return (second_args);
}

std::string ConfigParser::removeExcessiveSlashes(const std::string& path) {
	std::string result;
	bool was_last_slash = false;

	for (std::string::size_type i = 0; i < path.size(); ++i) {
		char c = path[i];

		if (c == '/') {
			if (!was_last_slash) {
				result += c;
				was_last_slash = true;
			}
		} else {
			result += c;
			was_last_slash = false;
		}
	}
    return (result);
}

std::string ConfigParser::simplifyPath(const std::string& path) {
    std::stack<std::string> stack;
    std::vector<std::string> components = split(path, '/');
    std::string simplifiedPath = "/";
    std::stack<std::string> reversedStack;
    bool isRelative = (path[0] != '/');
    for (std::vector<std::string>::size_type i = 0; i < components.size(); ++i) {
        if (components[i] == "." || components[i].empty()) {
            continue;
        } else if (components[i] == "..") {
            if (!stack.empty()) {
                stack.pop();
            }
        } else
            stack.push(components[i]);
    }
    while (!stack.empty()) {
        reversedStack.push(stack.top());
        stack.pop();
    }
    while (!reversedStack.empty()) {
        simplifiedPath += reversedStack.top();
        reversedStack.pop();
        if (!reversedStack.empty()) {
            simplifiedPath += "/";
        }
    }
    if (isRelative)
        simplifiedPath = "./" + (simplifiedPath.size() > 1 ? simplifiedPath.substr(1) : "");
    return (simplifiedPath);
}

std::string ConfigParser::toStrInt(int value) {
	std::ostringstream	oss;
	oss << value;
	return oss.str();
}

std::string ConfigParser::removeTrailingSlashes(const std::string& input) {
	std::string result = input;
	std::string::size_type end = result.size();
	while (end > 0 && result[end - 1] == '/') {
		--end;
	}
	result.resize(end);
	return (result);
}
