//Digital PS operation result code

typedef enum  
{  
	OP_SUCCESS = 0,  			//all of operation have return success
    RECV_DATA = 1,				//receive data from database error
    CREATE_SEND_SOCKET = 2,		//create the socket of sending data to ps error
    SET_SEND_SOCKET = 3,		//set the send socket error
    CONNECT_PS = 4,				//connect to PS error
    SEND_TO_PS = 5,				//send data to PS error
    RECV_RESP_PS =6,			//receive the response message from PS error
    RESP_PS = 7,				//the data format of the response from PS error
    DATA_PS = 8,				//ramping data format or data content error
    ORACLE_DATA = 9,            //oracle app send a error of ramping data 
    ORACLE_INVALID = 10,        //oracle app send a error of don't exist event talbe or version of digital PS
    ORACLE_EVT=11,              //oracle app send a error of event table
    ORACLE_PROCESSING=12,       //oracle app send a error of combine process of multi ramping data
    OTHERS = 13,				//other errors
    PS_CONTROLLER = 14          //injection BUMP PS controller connection error
}Operat_Code;