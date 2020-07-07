# Short description
In project implemented firmware for stm32 "blue pill" as USB CDC device with simple shell, that allows to configure board qpio in output mode in low and hi lewel, and in pwm mode

# Gpio terminal commands syntax

Full commands list:
```
        help|?
        gpio info
        pwm info
        gpio A|B|C|D port [0-15] mode on|off
        gpio A|B|C|D port [0-15] mode pwm [0-100]%
        pwm tim [1-3] freq <int>

```

- `help|?` - prints list of all commands
- `gpio info` - prints information about all gpio port configuration
- `pwm info` - prints info about timers freq and connected to this TIM ports
- `gpio A|B|C|D port [0-15] mode on|off` - set gpio X.x to hi or low state
- `gpio A|B|C|D port [0-15] mode pwm [0-100]%` - switch port to pwm mode and set duty in persents
  frequency is configuring for timer
- `pwm tim [1-3] freq <int>` - set freq in Hz for appropriate timer. Good values from 1 to 1000 Hz. Other frequency can make strage results

# How to build
This project is submodule for more common project https://github.com/collia/stm32scons.git and can be build inside of it.
