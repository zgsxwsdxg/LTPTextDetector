// Copyright (C) 2012  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#undef DLIB_IOSOCKSTrEAM_ABSTRACT_H__
#ifdef DLIB_IOSOCKSTrEAM_ABSTRACT_H__

#include "../sockstreambuf/sockstreambuf_abstract.h"

#include <iostream>

namespace dlib
{

// ---------------------------------------------------------------------------------------- 

    class iosockstream : public std::iostream
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This is an iostream object that reads/writes from a TCP network connection.

            THREAD SAFETY
                It is not safe to touch this object from more than one thread at a time.
                Therefore, you should mutex lock it if you need to do so.  
        !*/

    public:

        iosockstream(
        );
        /*!
            ensures
                - #good() == false
                - #tie() == this
                  (i.e. Any attempt to read from this stream will automatically flush the
                  stream's output buffers)
        !*/

        iosockstream( 
            const network_address& addr
        );
        /*!
            ensures
                - #tie() == this
                  (i.e. Any attempt to read from this stream will automatically flush the
                  stream's output buffers)
                - Attempts to connect to the given network address. 
                - Calling this constructor is equivalent to calling the default constructor
                  and then invoking open(addr).
                - #good() == true 
            throws
                - dlib::socket_error
                    This exception is thrown if there is some problem that prevents us from
                    creating the connection.  
        !*/

        iosockstream( 
            const network_address& addr,
            unsigned long timeout
        ); 
        /*!
            ensures
                - #tie() == this
                  (i.e. Any attempt to read from this stream will automatically flush the
                  stream's output buffers)
                - Attempts to connect to the given network address. 
                - Calling this constructor is equivalent to calling the default constructor
                  and then invoking open(addr, timeout).
                - #good() == true 
            throws
                - dlib::socket_error
                    This exception is thrown if there is some problem that prevents us from
                    creating the connection or if timeout milliseconds elapses before the
                    connect is successful.
        !*/

        ~iosockstream(
        );
        /*!
            ensures
                - Invokes close() before destructing the stream.  Therefore, any open
                  connection will be gracefully closed using the default timeout time.
                  This also means any data in the stream will be flushed to the connection.
        !*/

        void open (
            const network_address& addr
        );
        /*!
            ensures
                - This object will attempt to create a TCP connection with the remote host
                  indicated by addr.  
                - Any previous connection in this iosockstream is closed by calling close()
                  before we make any new connection.
                - #good() == true
                  (i.e. the error flags are reset by calling open())
            throws
                - dlib::socket_error
                    This exception is thrown if there is some problem that prevents us from
                    creating the connection.  
        !*/

        void open (
            const network_address& addr,
            unsigned long timeout 
        );
        /*!
            ensures
                - This object will attempt to create a TCP connection with the remote host
                  indicated by addr.  
                - Any previous connection in this iosockstream is closed by calling close()
                  before we make any new connection.
                - #good() == true
                  (i.e. the error flags are reset by calling open())
            throws
                - dlib::socket_error
                    This exception is thrown if there is some problem that prevents us from
                    creating the connection or if timeout milliseconds elapses before the
                    connect is successful.
        !*/

        void close(
            unsigned long timeout = 10000
        );
        /*!
            ensures
                - #good() == false 
                - if (there is an active TCP connection) then
                    - Flushes any data buffered in the output part of the stream
                      to the connection.  
                    - Performs a proper graceful close (i.e. like dlib::close_gracefully()).
                    - Will only wait timeout milliseconds for the buffer flush and graceful
                      close to finish before the connection is terminated forcefully.
                      Therefore, close() will only block for at most timeout milliseconds.
        !*/

        void terminate_connection_after_timeout (
            unsigned long timeout
        );
        /*!
            ensures
                - if (there is an active TCP connection) then
                    - Any operations on this TCP connection will return error or
                      end-of-file once timeout milliseconds have elapsed from this call to
                      terminate_connection_after_timeout().  This is true unless another
                      call to terminate_connection_after_timeout() is made which gives a
                      new time.  In this case, the previous call is forgotten and the
                      timeout is reset.
                    - This timeout only applies to the current TCP connection.  That is, if
                      the iosockstream is closed and a new connection is established, any
                      previous timeouts setup by terminate_connection_after_timeout() do
                      not apply. 
                - else
                    - This function has no effect on this object.
        !*/

    };

// ---------------------------------------------------------------------------------------- 

}


#endif // DLIB_IOSOCKSTrEAM_ABSTRACT_H__



