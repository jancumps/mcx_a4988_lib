module;

#include "fsl_gpio.h"

import stepper_driver;

export module a4988_mcx;

export namespace a4988_mcx {

class a4988_mcx: public stepper_driver::stepper_driver {
public:
    a4988_mcx( uint32_t n_enable, uint32_t ms1, uint32_t ms2,  uint32_t ms3, PORT_Type *gpio) :
        n_enable_(n_enable), ms1_(ms1), ms2_(ms2), ms3_(ms3), gpio_(gpio) {}
    virtual inline  bool init() override { 
        return true;
    }
    virtual bool microsteps(unsigned int microsteps) override { 
        unsigned int mode = true;
        switch (microsteps) {
        case 1:
            mode = 0x0000;
            break;
        case 2:
            mode = 0x0001;
            break;
        case 4:
            mode = 0x0002;
            break;
        case 8:
            mode = 0x0003;
            break;
        case 16:
            mode = 0x0007;
            break;
        default:
            // TODO check: assert(false); // develop check unsupported microstep
            mode = 0x0000;
        }
        put(ms1_, mode & 0b001);
        put(ms2_, mode & 0b010);
        put(ms3_, mode & 0b100);
        return true;
    }
    virtual void enable(bool enable) override {
        put(n_enable_, enable? false : true);
    }
    
private:
    uint32_t n_enable_;
    uint32_t ms1_;
    uint32_t ms2_;
    uint32_t ms3_;
    PORT_Type *gpio_;

    virtual void init_gpio() {
        // done by sysconfif
    }
    void put(uint32_t pin, bool set) {
        GPIO_PinWrite(gpio_, pin, set ? pin : 0U);
    }
}; // class a4988_mcx

} // namespace a4988_mcx