#include <iostream>
#include <thread>

#include "io.hpp"
#include "engine.hpp"

void Engine::accept(ClientConnection connection)
{
	auto thread = std::thread(&Engine::connection_thread, this, std::move(connection));
	thread.detach();
}

void Engine::connection_thread(ClientConnection connection)
{
	while(true)
	{
		ClientCommand input {};
		switch(connection.readInput(input))
		{
			case ReadResult::Error: SyncCerr {} << "Error reading input" << std::endl;
			case ReadResult::EndOfFile: return;
			case ReadResult::Success: break;
		}

		// Functions for printing output actions in the prescribed format are
		// provided in the Output class:
		switch(input.type)
		{
			case input_cancel: {
				SyncCerr {} << "Got cancel: ID: " << input.order_id << std::endl;

				// Remember to take timestamp at the appropriate time, or compute
				// an appropriate timestamp!
				// auto output_time = getCurrentTimestamp();
				// Output::OrderDeleted(input.order_id, true, output_time);
				CancelOrder cancel_order = CancelOrder(input.order_id);
				auto result = (Deleted*) orderbook.process_cancel(cancel_order).get_result().at(0); 

				Output::OrderDeleted(result->id, result->cancel_accepted, result->output_timestamp);
				break;
			}

			default: {
				SyncCerr {}
				    << "Got order: " << static_cast<char>(input.type) << " " << input.instrument << " x " << input.count << " @ "
				    << input.price << " ID: " << input.order_id << std::endl;

				// Remember to take timestamp at the appropriate time, or compute
				// an appropriate timestamp!
				Order order{input.type == input_buy? BUY: SELL, input.order_id, input.price, input.count, (uint32_t)getCurrentTimestamp(), input.instrument, 1};
				ResultWrapper results = orderbook.process_order(order);
				for (auto& result: results.get_result()) {
					switch(result->get_type()) {
						case ORDER_ADDED: {
							auto result_add = (Added*) result;
							Output::OrderAdded(result_add->id, (result_add->symbol).c_str(), result_add->price, result_add->count,
								result_add->is_sell_side, result_add->output_timestamp);
							break;
						}
						case ORDER_EXECUTED: {
							auto result_execute = (Executed*) result;
							Output::OrderExecuted(result_execute->resting_id, result_execute->new_id, result_execute->execution_id, 
								result_execute->price, result_execute->count, result_execute->output_timestamp);
							break;
						}
						default: {
							SyncCerr {} << "what is this" << std::endl;
						}
					}
				}
				break;
			}
		}

		// Additionally:

		// Remember to take timestamp at the appropriate time, or compute
		// an appropriate timestamp!
		// intmax_t output_time = getCurrentTimestamp();

		// Check the parameter names in `io.hpp`.
		// Output::OrderExecuted(123, 124, 1, 2000, 10, output_time);
	}
}
