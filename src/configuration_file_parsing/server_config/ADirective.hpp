#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

#include <set>
#include <iostream>
#include "../utils/PathValidator.hpp"

struct s_common_params {
	std::string				_root;
	std::set <std::string>	_index;
	bool					_auto_index;
	unsigned int			_client_max_body_size;
	bool					_is_index_files_init;
};

enum e_data_reach {
	DATA_OK = 0,
	DATA_NOK = 1,
	NO_DATA = 2
};

class ADirective {
	protected:
		PathValidator _validator;
		s_common_params _common_params;
	public:
		ADirective(void);
		virtual					~ADirective();
		virtual	std::ostream&	printAttributes(std::ostream& os) const;
		
		// Setters
		bool setRoot(std::string root_args);
		void setIndex(std::set <std::string> index_args);
		void setAutoIndex(bool value);
		void setClientMaxBodySize(unsigned int body_size_value);

		// Getters
		std::string				getRoot(void) const;
		std::set <std::string>	getIndex(void) const;
		bool					getAutoIndex(void) const;
		unsigned int			getClientMaxBodySize(void) const;
};

#endif