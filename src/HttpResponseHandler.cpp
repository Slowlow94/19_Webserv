#include "HttpResponseHandler.hpp"
#include "HttpRequestHandler.hpp"
#include "RequestResponseState.hpp"

HttpResponseHandler HttpResponseHandler::handleResponse(RRState &rrstate)
{
    try
    {
        *this = handlePath(rrstate);
        return *this; // Return the fully constructed response handler
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error constructing response: " << e.what() << std::endl;
        throw;
    }
}
