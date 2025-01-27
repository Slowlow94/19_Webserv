#include "TokenCounter.hpp"

TokenCounter::TokenCounter(void) {
	_blockStack.push(std::map<std::string, int>());
}

void TokenCounter::enterBlock(void) {
	_blockStack.push(std::map<std::string, int>());
}

void TokenCounter::exitBlock(void) {
	if (_blockStack.size() > 1) {
		_blockStack.pop();
	} else {
		std::cerr << "Error: Attempted to exit global block!" << std::endl;
	}
}

void TokenCounter::incrementToken(const std::string& token) {
	if (!_blockStack.empty()) {
		_blockStack.top()[token]++;
	}
}

int TokenCounter::getTokenCount(const std::string& token) const {
	if (!_blockStack.empty()) {
		std::map<std::string, int>::const_iterator it = _blockStack.top().find(token);
		if (it != _blockStack.top().end()) {
			return (it->second);
		}
	}
	return (0);
}

bool TokenCounter::oneOccurenceCheck(std::vector <std::string> unrepeatable_tokens) {
	for (std::size_t i = 0; i < unrepeatable_tokens.size(); i++) {
		if (getTokenCount(unrepeatable_tokens[i]) > 1)
			return (false);
	}
	return (true);
}

bool TokenCounter::isEmpty(void) const {
	return (_blockStack.top().size() == 0);
}