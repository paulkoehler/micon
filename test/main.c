/*
 * test.c
 *
 * Created: 06.06.2023 13:33:54
 * Author : paulk
 */
#define F_CPU 3686400

#include <avr/io.h>
#include <util/delay.h>

#define SWITCH0_FREQ 2093.00
#define SWITCH1_FREQ 2349.23
#define SWITCH2_FREQ 2637.02
#define SWITCH3_FREQ 2793.83
#define SWITCH4_FREQ 3135.96
#define SWITCH5_FREQ 3520.00
#define SWITCH6_FREQ 3951.07
#define SWITCH7_FREQ 4186.01

void initialize() {
	// PortD als Eingang mit Pullup-Widerständen
	DDRD = 0x00;    
	PORTD = 0xFF;   // Aktivieren der Pull-up Widerstände

	// PortC3 und PortC4 als Ausgang
	DDRC |= (1 << PC3) | (1 << PC4); 
}

void generateTone(float frequency) {
	if (frequency == 0.0) {
		
		// Wenn Frequenz 0 -> Ausgang = aus
		PORTC &= ~(1 << PC3) | ~(1 << PC4);  // PortC3 und PortC4 auf LOW setzen
		return;
	}

	// Anzahl der Taktzyklen pro Periode ermitteln
	unsigned int cycles_per_period = (unsigned int)(F_CPU / frequency);

	// Berechnen der Anzahl an Iterationen basierend auf der gewünschten Dauer
	unsigned int num_iterations = (unsigned int)((50.0 * frequency) / 1000.0);

	for (unsigned int i = 0; i < num_iterations; i++) {
		PORTC ^= (1 << PC3) | (1 << PC4);  // Umschalten von PortC3 und PortC4 für Tonerzeugung

		// Verzögerung basierend auf den Taktzyklen pro Periode
		for (unsigned int j = 0; j < cycles_per_period; j++) {
			asm volatile("nop");  // No operation to create a delay
		}
	}

	// Ausschalten der Ausgabe nach gewünschter Dauer
	PORTC &= ~(1 << PC3) | ~(1 << PC4);  // PortC3 und PortC4 auf LOW setzen
}

int main() {
	initialize();

	while (1) {
		// Abfrage Schalterzustand
		unsigned char switchState = PIND;

		// Spielen eines Tones basierend auf dem gedrückten Knopf
		if (!(switchState & (1 << PD0))) {
			generateTone(SWITCH0_FREQ);
			} else if (!(switchState & (1 << PD1))) {
			generateTone(SWITCH1_FREQ);
			} else if (!(switchState & (1 << PD2))) {
			generateTone(SWITCH2_FREQ); 
			} else if (!(switchState & (1 << PD3))) {
			generateTone(SWITCH3_FREQ); 
			} else if (!(switchState & (1 << PD4))) {
			generateTone(SWITCH4_FREQ);
			} else if (!(switchState & (1 << PD5))) {
			generateTone(SWITCH5_FREQ);
			} else if (!(switchState & (1 << PD6))) {
			generateTone(SWITCH6_FREQ);
			} else if (!(switchState & (1 << PD7))) {
			generateTone(SWITCH7_FREQ);
			} else {
			// Wenn kein Switch gedrückt ist, ausschalten
			generateTone(0.0);
		}

	}

	return 0;
}

