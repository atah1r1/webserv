#ifndef __SERVER_SETUP_HPP__
#define __SERVER_SETUP_HPP__

#include <iostream>
#include <vector>
#include "../../include/EnumRequestTarget.hpp"

typedef struct s_location
{
    std::string                                 path;  // Ex: /root/local/ ,path of location, oblig
    std::string                                 root;
    std::vector<std::string>                    index;
    std::vector<std::pair<short, std::string> > error_pages;
    int                                         client_max_body_size;
    std::vector<std::string>                    request_method;
    std::string                                 autoindex;

} t_location;

class ServerSetup
{
    friend class Parser;
    friend class Response;

    private:
    //------ public member --------- >>
    std::pair<short, u_int32_t>                 listen;                 // listen               80 0.0.0.0;
    std::vector<std::string>                    server_name;            // server_name          server1 server2;
    std::string                                 root;                   // root                 /var/www/example.com;
    std::vector<std::string>                    index;                  // index                index.html download.html;
    std::vector<std::pair<short, std::string> > error_pages;            // error_page           404 505 /custom_404.html /custom_505.html ;
    int                                         client_max_body_size;   // client_max_body_size 1024;
    std::vector<std::string>                    request_method;         // request_method       GET POST;
    std::string                                 autoindex;              // autoindex            off;
    std::vector<t_location>                     locations;              // locations            location /upload {}
    char***                                     envp;

    public:
    // --------------------------------------------------------- //
    // --------------- Constructors and Operators -------------- //
    // --------------------------------------------------------- //
   
    ServerSetup();
    ServerSetup(const ServerSetup& server_setup);
    ServerSetup&    operator=(const ServerSetup& server_setup);

    // --------------------------------------------------------- //
    // -------------------- Getters  --------------------------- //
    // --------------------------------------------------------- //
    std::pair<short, u_int32_t>                 getListen() const;
    std::vector<std::string>                    getServer_name() const;
    std::string                                 getRoot() const;
    std::vector<std::string>                    getIndex() const;  
    std::vector<std::pair<short, std::string> > getError_pages() const;
    int                                         getClient_max_body_size() const;
    std::vector<std::string>                    getRequest_method() const;        
    std::string                                 getAutoindex() const;             
    std::vector<t_location>                     getLocations() const; 
    char**                                      getEnvp() const;

    void                                        setEnvp(char*** envp);
    // --------------------------------------------------------- //
    // -------------------- Member Methods --------------------- //
    // --------------------------------------------------------- //
    t_location*                                 getLocation(std::string uri, TypeRequestTarget *type) const;
    t_location*                                 isLocation(std::string path, TypeRequestTarget *type) const;

    // --------------------------------------------------------- //
    // ----------------- Non Member Methods -------------------- //
    // --------------------------------------------------------- //
    static t_location initLocation();
    

};

#endif
