# cbEnchanted

cbEnchanted is a remake of [CoolBasic][] beta runtime. cbEnchanted is aiming to be harder, better, faster and stronger than current CoolBasic runtime and also one of the main goals is to support Linux, Windows and maybe Mac OS X, too.

The latest released and compiled version is found from [Downloads][]-section. General discussion and release announcements regarding cbEnchanted are at [CoolBasic Forums][]. We also have [documentation][] (sometimes a bit out-of-date) which we build from sources using [Doxygen][]. Doxygen also builds a nice [cbenchanted.qch][] file which you can plug in to your Qt Creator, if that's the IDE you're working with.

## Coding conventions

* Comment and code in english.
* Variables use camelCase while classes use CamelCase (capital letter).
* Indent with tabs.
* We use [Doxygen][] to handle our documentations. Please document your functions and classes as thoroughly as you can. Here's an example:

  ```c++
    /** Masks the object with the given color values
     * @param r The RED component of the mask color
     * @param g The GREEN component of the mask color
     * @param b The BLUE component of the mask color
     */
    void CBObject::maskObject(uint8_t r, uint8_t g, uint8_t b) {
  ```

* Write code that is easy to read. Use spaces to separate calculations, parameters, assignments, parts of if-statements, etc. Use common sense when naming your variables.

  ```c++
    // Don't do this:
    float b=a+cos((o->getAngle()/180.0)*M_PI)*(this->sizeX*this->sizeY);

    // Do this:
    float endX = startX + cos((obj->getAngle() / 180.0) * M_PI) * (this->sizeX * this->sizeY);
  ```

* Regarding block-level statements (`if`, `for`, `switch` and so on), put the starting brace to the same line as the closing parenthesis. Like this:

  ```c++
    for (int i = 0; i < 4; ++i) {
      layers[i] = new int32_t [mapWidth * mapHeight];
    }
  ```

* When in doubt, see the [Contact section][] below for ways to get in touch.

## Contact

We use IRC to keep in touch with each other. Drop by **#cbe** @ **IRCNet** to say hi. If you don't have an IRC client, [munIrkki.com][] has a nice, Java applet that you can use. Enter your nickname to the first field and #cbe to the second. If you feel like it, you can also contact VesQ via e-mail. Contact information is found on [VesQs profile page][].



  [coolbasic]: http://www.coolbasic.com
  [downloads]: http://latexi95.kapsi.fi/cbEnchanted/
  [coolbasic forums]: http://www.coolbasic.com/phpBB3/viewtopic.php?f=11&t=2702
                      "Topic of cbEnchanted on CoolBasic Forums"
  [documentation]: http://vesq.viuhka.fi/cbe/doc/
  [cbenchanted.qch]: http://vesq.viuhka.fi/cbe/doc/dl/cbenchanted.qch
                     "Qt Compressed Help file for cbEnchanted"
  [doxygen]: http://www.stack.nl/~dimitri/doxygen/
  [contact section]: #contact
  [munirkki.com]: http://www.munirkki.com/ircnet.php "MunIrkki.com, WebIRC, IRCNet"
  [vesqs profile page]: https://github.com/VesQ/