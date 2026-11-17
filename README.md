# Electronic oracles: magic, making and randomness

**We are making an electronic Magic 8 Ball with an Arduino, a tilt switch and the LCD screen. Then decide what your oracle should say, how it should respond and what kind of object it could become.**

## 1. What is a Magic 8 Ball?

![An illustrated classic Magic 8 Ball](assets/magic-eight-ball.png)

The Magic 8 Ball is a fortune telling toy. Someone asks a question, turns the black ball, and reads an answer through a window. Inside the traditional toy, a many sided die floats in liquid; whichever face settles against the window becomes the answer. It is a form of playful divination, not an object that knows the future.

What makes a prediction feel convincing? Is it the words, the way an answer is revealed, the act of asking, or the object itself? People have used many different objects and practices for divination, each with its own history. In this project, we will explore what happens when a simple electronic circuit takes on the role of an oracle.

### The example: Jicarilla Calabazo

![The Jicarilla Calabazo, a hand-painted Mexican gourd beside electronic components](assets/reference-jicarilla-calabazo.jpg)

The example brought to class is a **Jicarilla Calabazo**, a small, hand painted Mexican gourd vessel. Its rounded form, dark ground and floral decoration connect it to traditions of decorating gourds in Mexico. We do not know the maker or the precise community in which this particular object was made, and we are not presenting it as an object traditionally used for divination. It has its own material, artistic and cultural history before electronics enter the work.

Here, the electronic response is placed in conversation with a crafted surface and so the screen does not need to be the centre of the artwork and the gestures of holding with the turning and consulting the object can become part of the experience.

**Think about:** What might change if the same words appeared inside a hand painted object rather than a black plastic ball? How does an object invite us to interact with it?

## 2. What you need

- Arduino UNO R4 WiFi
- I²C LCD 1602 with four connection pins
- two-legged tilt switch
- jumper wires and a breadboard if needed
- USB-C cable and computer with Arduino IDE
- the `hd44780` library by Bill Perry
- optional paper and pen to sketch the form of your oracle

Note:  Keep the Arduino disconnected from power while moving wires.

**Start with the working simulation:** [Magic 8 Ball on Wokwi](https://wokwi.com/projects/480869321505295361).

Wokwi represents the tilt switch with a slide switch and simulates an Arduino UNO R3. In class, you will use the physical UNO R4 WiFi and its real tilt switch. The six connections and the programme work with both boards.

## 3. The tilt switch: the gesture that activates the oracle

![Illustration of a two-legged tilt switch with open and closed contacts](assets/tilt-switch.jpg)

The small black cylinder in your kit is a **tilt switch**. Inside is a movable metal contact, often a tiny ball. When the switch changes orientation, the contact can move and either join or separate two conducting parts. This is how an ordinary movement becomes an electrical signal.

The switch does **not** tell the Arduino the exact angle, how quickly you moved, or how hard you shook it. It only tells the board whether the electrical connection is **open** or **closed**.

We connect one leg to **D2** and the other to **GND**. The code uses `INPUT_PULLUP`, which makes D2 read `HIGH` when the switch is open and `LOW` when the switch connects D2 to ground. Either leg can connect to either side.

Think of the complete action as a chain:

**Turn the object → contact moves → Arduino detects a change → a response is selected → words appear.**

This small component can be used beyond a Magic 8 Ball. Imagine a sculptural object that speaks when lifted, an instrument that produces sounds when turned, a wearable that responds to a gesture, a game controlled by orientation, or a box that activates an alarm when tipped over. Each asks the artist to decide what a movement should *mean*.

**Think about:** Should the oracle be shaken, slowly rotated, bowed towards, held upside down, or passed between people? How might the gesture change the experience?

## 4. The I²C LCD: the voice of the oracle

An **LCD** is a liquid crystal display. The **1602** model has two rows, each with room for **16 characters**. Its I²C adapter allows the Arduino to send text through two signal connections, **SDA** for data and **SCL** for the clock, plus power and ground. The screen has no knowledge of the question. It displays the characters the programme gives it.

You can think of the screen as the **voice** of the oracle, but it is a very particular kind of voice. It speaks in short lines, with limited space, no rich typography and little room for explanation. That limitation can become an artistic decision.

Consider the difference between **YES**, **NOT YET**, **LOOK AGAIN** and **WHO IS ASKING?** These are not just different answers. They imply different personalities and relationships with the person consulting the object.

**Think about:** Where would you put the screen? How quickly should the words appear? Should the answer arrive all at once, remain hidden for a moment, or disappear after being read?

### What if the oracle spoke through sound instead?

Instead of displaying *YES*, perhaps the oracle could make a short bright sound. An uncertain answer might be a repeating rhythm, a slowly changing pitch or silence. In another project, the response could be communicated through light, movement or vibration.

With sound, you would need to think about **pitch, rhythm, volume, duration, silence and how people interpret what they hear**. Would everyone understand the same sound in the same way? Would sounds communicate more than words, or make the prediction more ambiguous?

Our first circuit uses the LCD. You do not need a speaker or buzzer to complete it, but you can carry these questions into later sound experiments.

## 5. Build the circuit

Use the component labels rather than relying on wire colours.

| Component connection | Arduino connection |
|---|---|
| LCD VCC | 5V |
| LCD GND | GND |
| LCD SDA | A4 |
| LCD SCL | A5 |
| Tilt switch leg 1 | D2 |
| Tilt switch leg 2 | GND |

Both GND connections can use separate ground pins or a shared breadboard ground rail. The tilt switch has no polarity.

**Test the circuit in [Wokwi](https://wokwi.com/projects/480869321505295361)** before assembling it with physical parts. In the simulation, the slide switch's middle pin connects to D2, one outer pin connects to GND and the other outer pin remains unused.

For the physical circuit, select **Arduino UNO R4 WiFi** and the correct port in Arduino IDE. Install `hd44780` by Bill Perry through Library Manager, then upload the same programme. If the LCD lights up but displays nothing legible, check the connections and try turning the small contrast potentiometer on its I²C adapter.

## 6. Read the programme

Here is the code already used by the [Wokwi project](https://wokwi.com/projects/480869321505295361). It is included in `sketch.ino` in this folder too.

```cpp
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

const int tilt = 2;
bool lastPressed = false;

const char* answers[] = {
  "Yes!",
  "No!",
  "Maybe",
  "Ask again",
  "Definitely!",
  "Not today"
};

void setup() {
  pinMode(tilt, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.backlight();

  randomSeed(analogRead(A0));

  lcd.print("MAGIC 8 BALL");
  lcd.setCursor(0, 1);
  lcd.print("Tilt to ask!");
}

void loop() {
  bool pressed = digitalRead(tilt) == LOW;

  if (pressed && !lastPressed) {
    lcd.clear();
    lcd.print("Thinking...");
    delay(500);

    int answer = random(0, 6);

    lcd.clear();
    lcd.print(answers[answer]);
  }

  lastPressed = pressed;
  delay(25);
}
```

There are four important ideas in this programme.

**Input:** `digitalRead(tilt)` checks whether the switch is closed. The condition `pressed && !lastPressed` asks whether it has *just* changed from open to closed. This stops a prediction being chosen on every repeated reading while the switch stays closed.

**A set of possibilities:** `answers[]` stores six phrases written by the programmer. The Arduino cannot invent a new answer outside this list.

**A random selection:** `random(0, 6)` chooses an integer from **0 to 5**. Each number points to a position in the list. This is *pseudorandom*: a computer algorithm generates a sequence that appears unpredictable, and `randomSeed(analogRead(A0))` helps choose where that sequence begins. It is not a supernatural prediction.

**Output:** `lcd.print(answers[answer])` places the selected phrase on the screen. The `delay(500)` creates a pause before the reveal. Even the pause is part of the artistic experience.

### What does this have to do with AI?

The electronic oracle chooses a **prewritten phrase** using pseudorandom numbers. A generative AI system works differently: it creates text using statistical patterns learned during training, often with probability-based choices between possible next tokens. It can respond to the words in a prompt, unlike our simple Arduino programme, but that is not the same as predicting the future.

**Who writes the possible answers? What does chance contribute? Why might someone believe or distrust a machine's response?** These questions matter for both a toy oracle and systems that appear more intelligent.

## 7. Experiment with the code

Change the programme, restart the Wokwi simulation or upload it again to the Arduino, and observe what happens. You can try these challenges in any order.

### Challenge 1. Write a different oracle

Replace the six phrases in `answers[]`. Give your oracle a personality: reassuring, suspicious, funny, poetic, contradictory or deliberately uncertain.

```cpp
const char* answers[] = {
  "Look closer",
  "Not yet",
  "Who is asking?",
  "Turn again",
  "Trust yourself",
  "It depends"
};
```

Keep six phrases for now, because the programme uses `random(0, 6)`. Keep each phrase to 16 characters or fewer. What kinds of questions does your oracle invite?

### Challenge 2. Change the moment of revelation

Find `delay(500);` and try `delay(2000);`. Replace `Thinking...` with `Listening...`, `Reading signs` or your own short phrase. Is a slow oracle more convincing, frustrating or theatrical?

### Challenge 3. Make the screen speak in two lines

Find the following part of the code:

```cpp
lcd.clear();
lcd.print(answers[answer]);
```

Replace it with:

```cpp
lcd.clear();
lcd.print("THE SIGNS SAY");
lcd.setCursor(0, 1);
lcd.print(answers[answer]);
```

What happens when the prediction is introduced as an authoritative statement? Try a title that undermines that authority instead.

### Challenge 4. Is randomness fair?

Keep six entries, but change three of them to `"Yes"`. Try the oracle several times. The programme still chooses from six positions with the same chance, but **half of those positions now say Yes**. The person who wrote the list has changed what is likely to be seen.

Does a random choice guarantee an unbiased result? How could an oracle be programmed to say what its designer wants people to hear?

### Challenge 5. Change the gesture

Move your physical tilt switch into different orientations and note when it closes the circuit. Can the object respond to being turned over rather than simply tapped? How would you place the switch inside an imagined sculpture so the movement feels deliberate?

### Challenge 6. Translate a prediction into sound

Choose three of your answers and describe how each could be expressed without words. Use rhythm, pitch, duration or silence. Could someone understand the answer without a written key? Could ambiguity be part of the work? This is a design challenge that can inform your later sound experiments; no extra wiring is required today.

## 8. Give your oracle a surface and a form

Return to the hand-painted gourd example. The colour, pattern, material, scale and way it is held all influence the experience before a message appears. You do not need to copy its decoration or its craft tradition. Instead, consider what materials, forms and making methods have meaning in your own design.

Sketch a possible object and think about these questions:

- Is the screen visible, partly hidden, or only revealed by a gesture?
- Does the object invite someone to turn, lift, cradle or pass it to another person?
- What relationship should there be between the physical surface and the electronic response?
- How would the experience change if the oracle communicated through sound or light rather than words?
- Who is the author of its predictions, and who gets to interpret them?

Your circuit can remain separate from the enclosure for now. There is no need to modify the hand-painted example.

## 9. Share what changed

Demonstrate one prediction, explain a change you made to the code, and show how you imagine your oracle becoming a physical object.

**To discuss:** If we know that the answers come from a programmed list, why can the encounter still feel magical? Is the magic in the software, the object, the gesture, or the person asking?

## Troubleshooting

- **Nothing appears on the LCD:** confirm 5V, GND, SDA to A4 and SCL to A5. On the physical display, adjust its contrast potentiometer.
- **The screen lights but prints no characters:** confirm the `hd44780` library is installed and the I²C connector is wired correctly.
- **There is no prediction:** turn the tilt switch into a different orientation. In Wokwi, move the slide switch to open and then close the connection.
- **The prediction appears more than once:** mechanical contacts can briefly chatter; allow the switch to settle before turning it again.
- **Your text is cut off:** each LCD line has only 16 character positions.
- **Wokwi and your board look different:** Wokwi is simulating an UNO R3; the class uses an UNO R4 WiFi. Select the UNO R4 WiFi when uploading to the physical device.

## Images and further reading

The Magic 8 Ball and component drawings in this README are original teaching illustrations. The photograph of the hand-painted gourd was supplied by the tutor; its maker and precise provenance are not confirmed.

- [Your Magic 8 Ball project on Wokwi](https://wokwi.com/projects/480869321505295361)
- [The Strong National Museum of Play: Magic 8 Ball](https://www.museumofplay.org/toys/magic-8-ball/)
- [Wokwi LCD 1602 reference](https://docs.wokwi.com/parts/wokwi-lcd1602)
- [Wokwi slide switch reference](https://docs.wokwi.com/parts/wokwi-slide-switch)
- [Arduino UNO R4 WiFi reference](https://docs.arduino.cc/tutorials/uno-r4-wifi/cheat-sheet/)
- [Arduino explanation of randomSeed](https://github.com/arduino/reference-en/blob/master/Language/Functions/Random%20Numbers/randomSeed.adoc)
