#ifndef LOCATION_BLOCK_HPP
# define LOCATION_BLOCK_HPP

#include <vector>
#include <iostream>
#include "ADirective.hpp"

typedef struct s_return_args {
	unsigned short status_code;
	std::set<std::string> multi_links;
}	t_return_args;

struct s_loc_params {
	std::string		_cgi_path;
	std::string		_uri;
	std::string		_content_path;
	std::string		_uri_dependance;
	std::string		_raw_uri_dependence;
	std::string		_alias_uri;
	unsigned char	_allowed_methods;
	bool			_modified_content_path;
	bool			_modified_client_max_body_size;
	bool			_modified_auto_index;
	bool			_return_set;
	s_return_args	_return_args;
};


enum e_allowed_methods {
	GET = 1 << 0,
	POST = 1 << 1,
	DELETE = 1 << 2
};

class LocationBlock : public ADirective {
	private:
		s_loc_params _location_params;
	public:
		LocationBlock(void);
		~LocationBlock();
		s_loc_params	getLocationParams(void) const;

		// Data reachability
		e_data_reach 							isContentPathReachable(void);
		e_data_reach 							isCgiPathReachable(void);
		std::pair<std::string, e_data_reach>	checkAvailableRessource(std::string file_path = "");
		std::pair<std::string, e_data_reach>	checkSubPathRessource(std::string subpath);

		// Setter
		void setUriDependance(std::string uri);
		void clientMaxBodySizeModified(void);
		void autoIndexModified(void);
		void setCgiPath(std::string path_args);
		bool setUri(std::string uri_args, std::string root);
		void setAlias(std::string alias_path);
		bool setAllowedMethods(unsigned char allowed_method);
		bool setContentPath(std::string content_path);
		void setRawUriDependence(const std::string uri);
		void setReturn(s_return_args return_arguments);

        // Getter
		std::string					getCgiPath(void) const;
		std::string					getUri(void) const;
		std::string					getContentPath(void) const;
		std::vector<std::string>	accessibleIndex(void);
		std::string					getAlias(void) const;
		bool						isCgiAllowed(void) const;
		bool						isContentPathModified(void) const;
		bool						isGetAllowed(void) const;
		bool						isPostAllowed(void) const;
		bool						isDeleteAllowed(void) const;
		bool						hasClientMaxBodySizeModified(void) const;
		bool						hasAutoIndexModified(void) const;
		std::string					getUriDependance(void) const;
		std::string					getRawUriDependence(void) const;
		s_return_args				getReturn(void) const;
		bool						isReturnSet(void) const;
};

std::ostream& operator<<(std::ostream& os, const LocationBlock *location_params);

#endif