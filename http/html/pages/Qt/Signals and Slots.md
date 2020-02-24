### Qt Signals and Slots

Signals and Slots are alternative to callbacks used in Qt. \
Signal emitted when event occurs. \
A slot is a function that is called in response to a particular signal.

The signals and slots mechanism is type safe: \
The signature of a signal must match the signature of the receiving slot.

A class which emits a signal neither knows nor cares which slots receive the signal.\
Qt's signals and slots mechanism ensures that if you connect a signal to a slot, the slot will be called with the signal's parameters at the right time. \
Signals and slots can take any number of arguments of any type. They are completely type safe.

Slots can be used for receiving signals, but they are also normal member functions. \
Just as an object does not know if anything receives its signals, a slot does not know if it has any signals connected to it. 

You can connect as many signals as you want to a single slot, and a signal can be connected to as many slots as you need. It is even possible to connect a signal directly to another signal. (This will emit the second signal immediately whenever the first is emitted.)

#### Signals
Signals are emitted by an object when its internal state has changed in some way that might be interesting to the object's client or owner. \
Signals are public access functions and can be emitted from anywhere.\
When a signal is emitted, the slots connected to it are usually executed immediately, just like a normal function call.\
If several slots are connected to one signal, the slots will be executed one after the other, in the order they have been connected, when the signal is emitted.\
Signals can never have return type - they always **void**.

#### Slots
A slot is called when a signal connected to it is emitted. Slots are normal C++ functions and can be called normally; their only special feature is that signals can be connected to them.\
Since slots are normal member functions, they follow the normal C++ rules when called directly. \
However, as slots, they can be invoked by any component, regardless of its access level, via a signal-slot connection. \
This means that a signal emitted from an instance of an arbitrary class can cause a private slot to be invoked in an instance of an unrelated class.\
You can also define slots to be *virtual*.

Compared to callbacks, signals and slots are slightly slower because of the increased flexibility they provide.\
In general, emitting a signal that is connected to some slots, is approximately **ten times** slower than calling the receivers directly, with non-virtual function calls

For example, simple C++ Counter class:
```cpp
class Counter
{
public:
    Counter() { m_value = 0; }

    int value() const { return m_value; }
    void setValue(int value);

private:
    int m_value;
};
```

In QObject based class implementation might look like:
```cpp
#include <QObject>

class Counter : public QObject
{
    Q_OBJECT

public:
    Counter() { m_value = 0; }

    int value() const { return m_value; }

public slots:
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};
```
The QObject-based version has the same internal state, and provides public methods to access the state, plus support for signals and slots.\
All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from *QObject*.

Typical *slot* might look like:
```cpp
void Counter::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);
    }
}
```
The emit line emits the signal *valueChanged()* from the object, with the new value as argument.

Usage:
```cpp

    Counter a, b;
    QObject::connect(&a, &Counter::valueChanged,
                     &b, &Counter::setValue);

    a.setValue(12);     // a.value() == 12, b.value() == 12
    b.setValue(48);     // a.value() == 12, b.value() == 48
    
```
Calling *a.setValue(12)* makes *a* emit a *valueChanged(12)* signal, which *b* will receive in its *setValue()* slot, i.e. *b.setValue(12)* is called. \
Then *b* emits the same *valueChanged()* signal, but since no slot has been connected to b's *valueChanged()* signal, the signal is ignored.