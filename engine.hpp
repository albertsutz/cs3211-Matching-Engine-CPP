// This file contains declarations for the main Engine class. You will
// need to add declarations to this file as you develop your Engine.

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <chrono>

#include "io.hpp"
#include "./orderbook/orderbook.hpp"

struct Engine
{
public:
	void accept(ClientConnection conn);
	Orderbook orderbook;
	Engine() = default;

private:
	void connection_thread(ClientConnection conn);
};

#endif
