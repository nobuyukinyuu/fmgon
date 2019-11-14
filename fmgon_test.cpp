//////////////////////////////////////////////////////////////////////////////
// fmtest.cpp --- test program of fmgon
// Copyright (C) 2015 Katayama Hirofumi MZ. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "soundplayer.h"

#include <string>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////


std::deque< std::string > Split(const std::string & input_string, const char delimiter) {
	std::stringstream         input_stream(input_string);
	std::string               string_element;
	std::deque< std::string > split_string;

	while (std::getline(input_stream, string_element, delimiter))
		split_string.emplace_back(string_element);

	return split_string;
} // Split


int main(int argc, char *argv[]) {
    alutInit(NULL, NULL);

    auto phrase = make_shared<VskPhrase>();

    phrase->m_setting.m_fm = true;

    if (argc <= 1) {
        phrase->m_setting.m_tone = 15;  // @15 DESCENT
    } else if (argc == 2) {
		phrase->m_setting.m_tone = atoi(argv[1]);
    } else {
		//return -1;
		phrase->m_setting.m_tone = -1;  // Manually specify the timbre

		//TODO:  check arg 2 to see if we're testing with LFO or not???
		//tokenize the input string in arg 3.
		std::string fm_envelope (argv[2]);
		std::deque<std::string> v = Split(fm_envelope, ',');

		int a[38];  //Arguments

		//Need at least 38 parameters to define the envelope.
		if (v.size() < 38) {
			printf("Argument 2:  Not enough parameters to define a program. %i", v.size());
			return -1;
		}

		for (int i=0; i<38; ++i) {
			/*std::cout << v.front();
			v.pop_front();*/

			a[i] = std::stoi(v.at(i));

		}

		
		// set the timbre.
		//input format: FL, AL,		AR1, DR1, SR1, RR1, SL1, TL1, KS1, MUL1, DT1,  ....... 

		phrase->m_setting.m_timbre.feedback = a[0];
		phrase->m_setting.m_timbre.algorithm = a[1];
		phrase->m_setting.m_timbre.opMask = MASK_ALL; //15.  All 4 operators enabled


		//LFO.  TODO.  We only care about the envelope right now, so use defaults
		phrase->m_setting.m_timbre.ams[OPERATOR_1] = 0;
		phrase->m_setting.m_timbre.ams[OPERATOR_2] = 0;
		phrase->m_setting.m_timbre.ams[OPERATOR_3] = 0;
		phrase->m_setting.m_timbre.ams[OPERATOR_4] = 0;
		phrase->m_setting.m_timbre.waveForm = 0;
		phrase->m_setting.m_timbre.sync = 0;
		phrase->m_setting.m_timbre.speed = 0;
		phrase->m_setting.m_timbre.pmd = 0;
		phrase->m_setting.m_timbre.amd = 0;
		phrase->m_setting.m_timbre.pms = 0;

		
		phrase->m_setting.m_timbre.setAR(a[2], a[11], a[20], a[29]);
		phrase->m_setting.m_timbre.setDR(a[3], a[12], a[21], a[30]);
		phrase->m_setting.m_timbre.setSR(a[4], a[13], a[22], a[31]);
		phrase->m_setting.m_timbre.setRR(a[5], a[14], a[23], a[32]);
		phrase->m_setting.m_timbre.setSL(a[6], a[15], a[24], a[33]);
		phrase->m_setting.m_timbre.setTL(a[7], a[16], a[25], a[34]);
		phrase->m_setting.m_timbre.setKS(a[8], a[17], a[26], a[35]);
		phrase->m_setting.m_timbre.setML(a[9], a[18], a[27], a[36]);
		phrase->m_setting.m_timbre.setDT(a[10], a[19], a[28], a[37]);

		//  OP-M has a second detune and flag for AMS enable in each OP.  Ignore.....

		//phrase->m_setting.m_timbre = ////
		//printf("Too many arguments, lol?%d\n%s", argc, argv[2]);
		//return -1;
	}

#if 1
    // CMD PLAY "@15T150L8O2CEGO3CEGO4CRRCO3GECO2GECR"
    phrase->m_setting.m_tempo = 150;
    // NOTE: 24 is the length of a quarter note

    phrase->m_setting.m_length = 8;
    phrase->m_setting.m_octave = 2;
    phrase->add_note('C');
	phrase->add_note('R');
	phrase->m_setting.m_octave = 3;
	phrase->add_note('C');
	phrase->add_note('R');
	phrase->add_note('D');
    phrase->add_note('F');
    phrase->add_note('A');
    phrase->m_setting.m_octave = 4;
    phrase->add_note('D');
    phrase->add_note('R');
    phrase->add_note('R');
	phrase->m_setting.m_length = 12;
	phrase->m_setting.m_octave = 5;
	phrase->add_note('D');

 //   phrase->add_note('C');
 //   phrase->m_setting.m_octave = 4;
 //   phrase->add_note('G');
 //   phrase->add_note('E');
 //   phrase->add_note('C');
 //   phrase->m_setting.m_octave = 3;
 //   phrase->add_note('G');
 //   phrase->add_note('E');
 //   phrase->add_note('C');
 //   phrase->add_note('R');
	//phrase->add_note('R');

#else
    // KAERU NO UTA
    // CMD PLAY "@15T150O4L4CDEFEDCREFGAGFERCRCRCRCRL8CCDDEEFFL4EDCR"
    phrase->m_setting.m_tempo = 120;
    phrase->m_setting.m_octave = 3;
    // NOTE: 24 is the length of a quarter note
    phrase->m_setting.m_length = 24;

    phrase->add_note('C');
    phrase->add_note('D');
    phrase->add_note('E');
    phrase->add_note('F');
    phrase->add_note('E');
    phrase->add_note('D');
    phrase->add_note('C');
    phrase->add_note('R');

    phrase->add_note('E');
    phrase->add_note('F');
    phrase->add_note('G');
    phrase->add_note('A');
    phrase->add_note('G');
    phrase->add_note('F');
    phrase->add_note('E');
    phrase->add_note('R');

    phrase->add_note('C');
    phrase->add_note('R');
    phrase->add_note('C');
    phrase->add_note('R');
    phrase->add_note('C');
    phrase->add_note('R');
    phrase->add_note('C');
    phrase->add_note('R');

    phrase->m_setting.m_length = 12;
    phrase->add_note('C');
    phrase->add_note('C');
    phrase->add_note('D');
    phrase->add_note('D');
    phrase->add_note('E');
    phrase->add_note('E');
    phrase->add_note('F');
    phrase->add_note('F');
    phrase->m_setting.m_length = 24;
    phrase->add_note('E');
    phrase->add_note('D');
    phrase->add_note('C');
    phrase->add_note('R');
#endif

    VskScoreBlock block;
    block.push_back(phrase);

    VskSoundPlayer player;
    player.play_and_wait(block);

    alutExit();

    return 0;
} // main

//////////////////////////////////////////////////////////////////////////////

