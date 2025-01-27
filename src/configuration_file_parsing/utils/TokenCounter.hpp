#ifndef TOKEN_COUNTER_HPP
# define TOKEN_COUNTER_HPP

#include <map>
#include <stack>
#include <vector>
#include <iostream>

class TokenCounter {
	private:
		std::stack<std::map<std::string, int> > _blockStack;
	public:
		TokenCounter(void);

		void	enterBlock(void);
		void	exitBlock(void);
		void	incrementToken(const std::string& token);
		int		getTokenCount(const std::string& token) const;
		bool	oneOccurenceCheck(std::vector <std::string> unrepeatable_tokens);
		bool	isEmpty(void) const;
};

#endif