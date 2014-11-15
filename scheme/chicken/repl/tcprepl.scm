#| RUNNING A CHICKEN REPL OVER A SOCKET

Possibly better implementations exist - for example using  the generic tcpserver. You might also consider using the egg tcp-server

http://www.irp.oist.jp/trac/chicken/browser/wiki/Connecting%20to%20a%20REPL%20via%20sockets

It is important to start the listening executable with the {{-:c}}
Runtime-option, to force output of the prompt (even if not communicating
with a terminal port).
	
If you compile and run this program, you should be able to connect
to it via {{telnet}}:

 % telnet localhost 7114
 Trying ::1...
 telnet: connect to address ::1: Connection refused
 Trying 127.0.0.1...
 Connected to localhost.
 Escape character is '^'.
 #;> 123
 123

 Additional note 08-Mar-2010: Consider using tcpserver instead. For
 cygwin, this can be obtained from the ucspi-tcp package
 
|#


(use tcp)
	
(define (remote-repl #!optional (port 7114))
  (display (format "Serving repl on port ~a~%" port))
  (let*-values (((x) (tcp-listen port))
		((i o) (tcp-accept x)))
    (current-input-port i)
    ;(current-output-port o)

    ;(current-error-port o)
					;(when (provided? 'debug) (set! ##dbg#command-output-port o))   ; in case you use the debug egg
    (repl)))

(remote-repl)
