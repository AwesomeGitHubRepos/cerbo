#pragma once

#include "inputs.hpp"
#include "stend.hpp"


//void eaug_main(inputs_t& inputs, const stend_ts& stends);
detran_cs eaug_main(const etran_ts& etrans, const stend_ts& stends, 
		const period &per);
