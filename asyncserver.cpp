#include <iostream>
#include <ostream>
#include <istream>
#include <ctime>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
//g++ async_http_server.cpp -o async_http_server -lboost_system -lboost_thread -lpthread
using boost::asio::ip::tcp;

class HttpServer; // forward declaration

class Request : public boost::enable_shared_from_this<Request>
{
    static std::string make_daytime_string()
    {
          std::time_t now = std::time(0);
          return std::ctime(&now);
    }

    // member variables
    HttpServer& server;
    boost::asio::streambuf request;

    void afterRead( const boost::system::error_code& ec, std::size_t bytes_transferred)
    {
        // done reading, writes answer (yes, we ignore the request);
        boost::asio::streambuf response;
        std::ostream res_stream(&response);

        // gets daytime string
        std::string time = make_daytime_string();

        res_stream << "HTTP/1.0 200 OK\r\n"
                   << "Content-Type: text/html; charset=UTF-8\r\n"
                   << "Content-Length: " << time.length() + 2 << "\r\n\r\n"
                   << time << "\r\n";
        boost::asio::async_write(*socket, response, boost::bind(&Request::afterWrite, shared_from_this(), _1, _2));
    }
    
    void afterWrite( const boost::system::error_code& ec, std::size_t bytes_transferred)
    {
        // done writing, closing connection
        socket->close();
    }    

    public:

        boost::shared_ptr<tcp::socket> socket;
        Request(HttpServer& server);
        void answer()
        {
            if (!socket) return;
            
            // reads request till the end
            boost::asio::async_read_until(*socket, request, "\r\n\r\n",
                                          boost::bind(&Request::afterRead, shared_from_this(), _1, _2));
        }
        
};


class HttpServer
{
    public:
    
    HttpServer(unsigned int port) : acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {}
    ~HttpServer() { if (sThread) sThread->join(); }
    
    void Run()
    {
        sThread.reset(new boost::thread(boost::bind(&HttpServer::thread_main, this)));
    }

    boost::asio::io_service io_service;

    private:
    tcp::acceptor acceptor;
    boost::shared_ptr<boost::thread> sThread;

    void thread_main()
    {
        // adds some work to the io_service
        start_accept();
        io_service.run();
    }

    void start_accept()
    {
        boost::shared_ptr<Request> req (new Request(*this));
        acceptor.async_accept(*req->socket,
                               boost::bind(&HttpServer::handle_accept, this, req, _1));
    }

    void handle_accept(boost::shared_ptr<Request> req, const boost::system::error_code& error)
    {
        if (!error) { req->answer(); }
        start_accept();
    }
};

Request::Request(HttpServer& server): server(server)
{
    socket.reset(new tcp::socket(server.io_service));
}


int main (int argc, char *argv[])
{
   HttpServer server(8080);
   server.Run();
}