// TODO
//	What if functions are called afer socket is deleted?
//	What if WebSocket data received or sent is binary?
//	Create listener for WebSocket error().
//	https://emscripten.org/docs/porting/asyncify.html#making-async-web-apis-behave-as-if-they-were-synchronous

#pragma once

#ifndef __EMSCRIPTEN__
	#error Emscripten required.
#endif

#include <queue>
#include <string>

#include <emscripten.h>

namespace cppquery {
	void initialize( void ) {};

	namespace http {
		void initialize( void ) {};

		void fetch( const char* uri, const char* method = "GET", const char* data = NULL ) {
			EM_ASM( {
				async () => {
					const response = await fetch( UTF8ToString( $0 ), {
						method:	UTF8ToString( $1 ),
						body:	UTF8ToString( $2 )
					} );

					return await response.text();
				};
			}, uri, method, data );
		};
	};

	namespace html {
		void initialize( void ) {};

		void createElement( const char* elementType, const char* parentQuery = "body", const char* idName = NULL ) {
			EM_ASM( {
				newElement = document.createElement( UTF8ToString( $0 ) );
				document.querySelector( UTF8ToString( $1 ) ).appendChild( newElement );

				if ( $2 !== 0 )
					newElement.setAttribute( "id", UTF8ToString( $2 ) );
			}, elementType, parentQuery, idName );
		};

		void createElementText( const char* parentElementId, const char* text ) {
			EM_ASM( {
				newElement = document.createTextNode( UTF8ToString( $1 ) );
				document.querySelector( UTF8ToString( $0 ) ).append( newElement );
			}, parentElementId, text );
		};

		void setElementId( const char* query, const char* idName ) {
			EM_ASM( {
				document.querySelector( UTF8ToString( $0 ) ).setAttribute( "id", UTF8ToString( $1 ) );
			}, query, idName );
		};

		void addElementClass( const char* query, const char* className ) {
			EM_ASM( {
				document.querySelector( UTF8ToString( $0 ) ).classList.add( UTF8ToString( $1 ) );
			}, query, className );
		};

		void removeElementClass( const char* query, const char* className ) {
			EM_ASM( {
				document.querySelector( UTF8ToString( $0 ) ).classList.remove( UTF8ToString( $1 ) );
			}, query, className );
		};

		void appendElement( const char* query, const char* elementName ) {
			EM_ASM( {
				document.querySelector( UTF8ToString( $0 ) ).appendChild( document.querySelector( UTF8ToString( $1 ) ) );
			}, query, elementName );
		};

		int getBodyWidth( void ) {
			return EM_ASM_INT( {
				return document.body.clientWidth;
			} );
		};

		int getBodyHeight( void ) {
			return EM_ASM_INT( {
				return document.body.clientHeight;
			} );
		};
	};

	namespace css {
		void initialize( void ) {};
	};

	namespace websocket {
		enum STATE {
			CONNECTING = 0,
			OPENED,
			CLOSING,
			CLOSED
		};

		void initialize( void ) {
			EM_ASM( {
				javascriptWebsocketSetMessageInboundMessage = Module.cwrap( "cppWebsocketSetMessageInboundMessage", null, "string" );
			} );
		};

		std::queue< const std::string > _incomingMessages;

		EM_JS( void, javascriptWebsocketSetMessageOutbound, ( const char* message ), {
			_websocket.send( UTF8ToString( message ) );
		} );

		extern "C" {
			EMSCRIPTEN_KEEPALIVE
			void cppWebsocketSetMessageInboundMessage( const char* message ) {
				_incomingMessages.push( message );
			};
		};

		int getState( void ) {
			return EM_ASM_INT( {
				return _websocket.readyState;
			} );
		};

		EM_JS( void, openSocket, ( const char* uri ), {
			_websocket = new WebSocket( UTF8ToString( uri ) );

			_websocket.addEventListener( "message", ( message ) => {
				const sendMessage = ( text ) => {
					const length = lengthBytesUTF8( text ) + 1;
					const cString = _malloc( length );
					stringToUTF8( text, cString, length );
					javascriptWebsocketSetMessageInboundMessage( cString );
					_free( cString );
				};

				if ( message.data instanceof Blob ) {
					const reader = new FileReader();
					reader.addEventListener( "loadend", ( event ) => {
						sendMessage( event.srcElement.result );
					} );

					reader.readAsText( message.data );
				}
				else
					sendMessage( message.data );
			} );
		} );

		EM_JS( void, closeSocket, (), {
			_websocket.close();

			delete _websocket;
		} );

		std::size_t messagesCount( void ) {
			return _incomingMessages.size();
		};

		std::string getMessage( void ) {
			const std::string message = _incomingMessages.front();
			_incomingMessages.pop();

			return message;
		};

		void sendMessage( const char* messsage ) {
			javascriptWebsocketSetMessageOutbound( messsage );
		};

		void sendMessage( const std::string messsage ) {
			sendMessage( messsage.c_str() );
		};

		void sendMessage( const int messsage ) {
			sendMessage( std::to_string( messsage ) );
		};

		void sendMessage( const unsigned int messsage ) {
			sendMessage( std::to_string( messsage ) );
		};

		void sendMessage( const std::size_t messsage ) {
			sendMessage( std::to_string( messsage ) );
		};

		void sendMessage( const float messsage ) {
			sendMessage( std::to_string( messsage ) );
		};

		void sendMessage( const double messsage ) {
			sendMessage( std::to_string( messsage ) );
		};
	};
};
