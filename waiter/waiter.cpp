#include <string>
#include "stdlib.h"

#include "../includes/externs.h"
#include "../includes/waiter.h"
#include "../includes/PRINT.h"
using namespace std;

mutex waiterMutex;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	//lock guard so no two waiters grab the same order
	//lock_guard<mutex> lock(waiterMutex);
	return myIO.getNext(anOrder);;
}

void Waiter::beWaiter() {

	int next = SUCCESS;
//	b_WaiterIsFinished = false;
	//gets new orders until getNext runs out, then signals b_WaiterIsFinished
	while (next == SUCCESS){
		ORDER temp;
		next = getNext(temp);
		if (next == SUCCESS){
			unique_lock<mutex> lck(mutex_order_inQ);
			PRINT4("Be waiter ", id, " pushing order number: ", temp.order_number);
			order_in_Q.push(temp);
		} else {
			{
				unique_lock<mutex> lck(mutex_order_inQ);
				b_WaiterIsFinished = true;
				PRINT3("Waiter ", id ," notify baker");
			}
			cv_order_inQ.notify_all();
		}
		//signals Bakers that a new order is in queue or waiter is done
		PRINT3("Waiter ", id ," notify baker");
		cv_order_inQ.notify_all();

	}

}

