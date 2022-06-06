#include "include/ServerSetup.hpp"
#include "../include/Utils.hpp"
// --------------------------------------------------------- //
// --------------- Constructors and Operators -------------- //
// --------------------------------------------------------- //
ServerSetup::ServerSetup()
{
    this->listen = std::pair<short, u_int32_t> (); listen.first = -1;
    this->server_name = std::vector<std::string>();
    this->root = std::string();
    this->index = std::vector<std::string>();
    this->error_pages = std::vector<std::pair<short, std::string> >();
    this->client_max_body_size = -1;
    this->request_method =  std::vector<std::string>();
    this->autoindex = std::string();
    this->locations = std::vector<t_location>();
    this->envp = NULL;
}

ServerSetup::ServerSetup(const ServerSetup& server_setup)
{
    *this = server_setup;
}

ServerSetup&    ServerSetup::operator=(const ServerSetup& server_setup)
{
    this->listen = server_setup.listen;
    this->server_name = server_setup.server_name;
    this->root = server_setup.root;
    this->index = server_setup.index;
    this->error_pages = server_setup.error_pages;
    this->client_max_body_size = server_setup.client_max_body_size;
    this->request_method = server_setup.request_method;
    this->autoindex = server_setup.autoindex;
    this->locations = server_setup.locations;
    this->envp = server_setup.envp;
    return (*this);
}

// --------------------------------------------------------- //
// -------------------- Getters  --------------------------- //
// --------------------------------------------------------- //
std::pair<short, u_int32_t>                 ServerSetup::getListen() const
{
    return (this->listen);
}

std::vector<std::string>                    ServerSetup::getServer_name() const
{
    return (this->server_name);
}

std::string                                 ServerSetup::getRoot() const
{
    return (this->root);
}

std::vector<std::string>                    ServerSetup::getIndex() const
{
    return (this->index);
}

std::vector<std::pair<short, std::string> > ServerSetup::getError_pages() const
{
    return (this->error_pages);
}

int                                         ServerSetup::getClient_max_body_size() const
{
    return (this->client_max_body_size);
}

std::vector<std::string>                    ServerSetup::getRequest_method() const
{
    return (this->request_method);
}  

std::string                                 ServerSetup::getAutoindex() const
{
    return (this->autoindex);
}  

std::vector<t_location>                     ServerSetup::getLocations() const
{
    return (this->locations);
}

char**                                      ServerSetup::getEnvp() const
{
    return (*(this->envp));
}

void                                        ServerSetup::setEnvp(char*** envp)
{
    this->envp = envp;
}

// --------------------------------------------------------- //
// -------------------- Member Methods --------------------- //
// --------------------------------------------------------- //
t_location*                                 ServerSetup::isLocation(std::string path, TypeRequestTarget *type) const
{
    for (size_t i = 0; i < getLocations().size(); i++)
        if (getLocations()[i].path == path)
            {
                *type = IS_LOCATION;
                t_location *location = new t_location();
                *location = getLocations()[i];
                return (location);
            }
    return (NULL);
}

t_location*                                  ServerSetup::getLocation(std::string uri, TypeRequestTarget *type) const
{   
    std::string path = getRoot() + uri;
    TypeRequestTarget type_request;
    t_location* location;
    if ((type_request = getPathType(path)) == IS_FILE)
    {
        *type = IS_FILE;
        return (NULL);
    }
    // Chank the URI directories /../.../..
    path = uri;
    if (this->getAutoindex() == "off")
    {
        while (coutChar(path, '/') > 0) // plus two directory
        {   
            if ((location = isLocation(path, type)) != NULL)
                return (location);
            path = path.substr(0, path.find_last_of('/'));
        }
    }
    else
    {
        if ((location = isLocation(path, type)) != NULL)
            return (location);
    }
   
    if (type_request == IS_DIRECTORY)
        *type = IS_DIRECTORY;
    else
        *type = IS_NOT_FOUND;
    return (NULL);
}

// --------------------------------------------------------- //
// ---------------- Non Member Functions ------------------- //
// --------------------------------------------------------- //
t_location ServerSetup::initLocation()
{
    t_location location;
    location.path = std::string();
    location.root = std::string();
    location.index = std::vector<std::string>();
    location.error_pages = std::vector<std::pair<short, std::string> >();
    location.client_max_body_size = -1;
    location.request_method =  std::vector<std::string>();
    location.autoindex = std::string();
    return (location);
}

