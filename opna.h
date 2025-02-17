// ---------------------------------------------------------------------------
//  OPN/A/B interface with ADPCM support
//  Copyright (C) cisc 1998, 2003.
// ---------------------------------------------------------------------------
//  $Id: opna.h,v 1.33 2003/06/12 13:14:37 cisc Exp $

#ifndef FM_OPNA_H
#define FM_OPNA_H

#include "fmgen.h"
#include "fmtimer.h"
#include "psg.h"

// ---------------------------------------------------------------------------
//  class OPN/OPNA
//  OPN/OPNA に良く似た音を生成する音源ユニット
//	Sound generator unit that produces sounds similar to OPN / OPNA.
//
//  interface:
//  bool Init(uint32_t clock, uint32_t rate, bool, const char* path);
//      Initialization. Please call it before using this class.
//		For OPNA, use this function to read the rhythm sample.
//
//	clock:	OPN / OPNA / OPNB clock frequency (Hz)
//
//	rate:	Generated PCM sample frequency (Hz)
//
//	path:	Rhythm sample path (only valid for OPNA)
//			If omitted, read from current directory
//			Add '\' or '/' to the end of the string
//
//			Returns true if initialization is successful
//
// bool LoadRhythmSample (const char * path)
//		(OPNA ONLY)
//		Reread the Rhythm sample.
//		path is the same as Init's path.
//
// bool SetRate (uint32_t clock, uint32_t rate, bool)
//		Change clock or PCM rate.
//		See Init for arguments etc.
//
// void Mix (FM_SAMPLETYPE * dest, int nsamples)
//		Synthesize Stereo PCM data for nsamples and added to the array starting with dest.
//		(Addition)
// � dest needs an area of ??sample * 2
// � The storage format is L, R, L, R ...
// � Because it is just addition, it is necessary to clear the array to zero beforehand
// � If FM_SAMPLETYPE is short type, clipping is performed.
// � This function is independent of the internal timer.
// Timer needs to be manipulated with Count and GetNextEvent.
//
// void Reset ()
// reset (initialize) the sound source
//
// void SetReg (uint32_t reg, uint32_t data)
// Write data to the register reg of the sound source
//
// uint32_t GetReg (uint32_t reg)
// Read the contents of the register reg of the sound source
// Readable registers are part of PSG, ADPCM, ID (0xff)
//
// uint32_t ReadStatus () / ReadStatusEx ()
// Read sound source status register
// ReadStatusEx reads the extended status register (OPNA)
// busy flag is always 0
//
// bool Count (uint32_t t)
// Advance the sound source timer by t [?s].
// When the internal state of the sound source has changed (timer overflow)
// returns true
//
// uint32_t GetNextEvent ()
// Necessary until one of the sound source timers overflows
// returns time [?s]
// If the timer is stopped, return ULONG_MAX ...
//
// void SetVolumeFM (int db) / SetVolumePSG (int db) ...
// Adjust the volume of each sound source in the +-direction. The standard value is 0.
// Unit is about 1/2 dB, upper limit of effective range is 20 (10dB)
//

namespace FM {
    //  OPN Base -------------------------------------------------------
    class OPNBase : public Timer {
    public:
        OPNBase();

        bool        Init(uint32_t c, uint32_t r);
        virtual void Reset();

        void        SetVolumeFM(int db);
        void        SetVolumePSG(int db);
        void        SetLPFCutoff(uint32_t freq) {}  // obsolete

    protected:
        void        SetParameter(Channel4* ch, uint32_t addr, uint32_t data);
        void        SetPrescaler(uint32_t p);
        void        RebuildTimeTable();

        int         fmvolume;

        uint32_t    clock;              // OPN クロック
        uint32_t    rate;               // FM 音源合成レート
        uint32_t    psgrate;            // FMGen  出力レート
        uint32_t    status;
        Channel4 *  csmch;

        static uint32_t lfotable[8];

    private:
        void        TimerA();
        uint8_t     prescale;

    protected:
        Chip        chip;
        PSG         psg;
    }; // class OPNBase

    //  OPN2 Base ------------------------------------------------------
    class OPNABase : public OPNBase {
    public:
        OPNABase();
        ~OPNABase();

        uint32_t    ReadStatus() {
            return status & 0x03;
        }
        uint32_t    ReadStatusEx();
        void        SetChannelMask(uint32_t mask);

    private:
        virtual void Intr(bool) {}

        void        MakeTable2();

    protected:
        bool        Init(uint32_t c, uint32_t r, bool);
        bool        SetRate(uint32_t c, uint32_t r, bool);

        void        Reset();
        void        SetReg(uint32_t addr, uint32_t data);
        void        SetADPCMBReg(uint32_t reg, uint32_t data);
        uint32_t    GetReg(uint32_t addr);

    protected:
        void        FMMix(Sample* buffer, int nsamples);
        void        Mix6(Sample* buffer, int nsamples, int activech);

        void        MixSubS(int activech, ISample**);
        void        MixSubSL(int activech, ISample**);

        void        SetStatus(uint32_t bit);
        void        ResetStatus(uint32_t bit);
        void        UpdateStatus();
        void        LFO();

        void        DecodeADPCMB();
        void        ADPCMBMix(Sample* dest, uint32_t count);

        void        WriteRAM(uint32_t data);
        uint32_t    ReadRAM();
        int         ReadRAMN();
        int         DecodeADPCMBSample(uint32_t);

    // FM 音源関係
        uint8_t     pan[6];
        uint8_t     fnum2[9];

        uint8_t     reg22;
        uint32_t    reg29;      // OPNA only?

        uint32_t    stmask;
        uint32_t    statusnext;

        uint32_t    lfocount;
        uint32_t    lfodcount;

        uint32_t    fnum[6];
        uint32_t    fnum3[3];

    // ADPCM 関係
        uint8_t*    adpcmbuf;       // ADPCM RAM
        uint32_t    adpcmmask;      // メモリアドレスに対するビットマスク
        uint32_t    adpcmnotice;    // ADPCM 再生終了時にたつビット
        uint32_t    startaddr;      // Start address
        uint32_t    stopaddr;       // Stop address
        uint32_t    memaddr;        // 再生中アドレス
        uint32_t    limitaddr;      // Limit address/mask
        int         adpcmlevel;     // ADPCM 音量
        int         adpcmvolume;
        int         adpcmvol;
        uint32_t    deltan;         // �儂
        int         adplc;          // 周波数変換用変数
        int         adpld;          // 周波数変換用変数差分値
        uint32_t    adplbase;       // adpld の元
        int         adpcmx;         // ADPCM 合成用 x
        int         adpcmd;         // ADPCM 合成用 ��
        int         adpcmout;       // ADPCM 合成後の出力
        int         apout0;         // out(t-2)+out(t-1)
        int         apout1;         // out(t-1)+out(t)

        uint32_t    adpcmreadbuf;   // ADPCM リード用バッファ
        bool        adpcmplay;      // ADPCM 再生中
        int8_t      granuality;
        bool        adpcmmask_;

        uint8_t     control1;       // ADPCM コントロールレジスタ１
        uint8_t     control2;       // ADPCM コントロールレジスタ２
        uint8_t     adpcmreg[8];    // ADPCM レジスタの一部分

        int         rhythmmask_;

        Channel4    ch[6];

        static void BuildLFOTable();
        static int amtable[FM_LFOENTS];
        static int pmtable[FM_LFOENTS];
        static int32_t tltable[FM_TLENTS+FM_TLPOS];
        static bool tablehasmade;
    };

    //  YM2203(OPN) ----------------------------------------------------
    class OPN : public OPNBase {
    public:
        OPN();
        virtual ~OPN() {}

        bool        Init(uint32_t c, uint32_t r, bool=false, const char* =0);
        bool        SetRate(uint32_t c, uint32_t r, bool=false);

        void        Reset();
        void        Mix(Sample* buffer, int nsamples);
        void        SetReg(uint32_t addr, uint32_t data);
        uint32_t    GetReg(uint32_t addr);
        uint32_t    ReadStatus() { return status & 0x03; }
        uint32_t    ReadStatusEx() { return 0xff; }

        void        SetChannelMask(uint32_t mask);

        int         dbgGetOpOut(int c, int s) { return ch[c].op[s].dbgopout_; }
        int         dbgGetPGOut(int c, int s) { return ch[c].op[s].dbgpgout_; }
        Channel4*   dbgGetCh(int c) { return &ch[c]; }

    private:
        virtual void Intr(bool) {}

        void        SetStatus(uint32_t bit);
        void        ResetStatus(uint32_t bit);

        uint32_t    fnum[3];
        uint32_t    fnum3[3];
        uint8_t     fnum2[6];

        Channel4    ch[3];
    }; // class OPN

    //  YM2608(OPNA) ---------------------------------------------------
    class OPNA : public OPNABase {
    public:
        OPNA();
        virtual ~OPNA();

        bool        Init(uint32_t c, uint32_t r, bool  = false, const char* rhythmpath=0);
        bool        LoadRhythmSample(const char*);

        bool        SetRate(uint32_t c, uint32_t r, bool = false);
        void        Mix(Sample* buffer, int nsamples);

        void        Reset();
        void        SetReg(uint32_t addr, uint32_t data);
        uint32_t    GetReg(uint32_t addr);

        void        SetVolumeADPCM(int db);
        void        SetVolumeRhythmTotal(int db);
        void        SetVolumeRhythm(int index, int db);

        uint8_t*    GetADPCMBuffer() { return adpcmbuf; }

        int         dbgGetOpOut(int c, int s) { return ch[c].op[s].dbgopout_; }
        int         dbgGetPGOut(int c, int s) { return ch[c].op[s].dbgpgout_; }
        Channel4*   dbgGetCh(int c) { return &ch[c]; }


    private:
        struct Rhythm {
            uint8_t     pan;        // ぱん
            int8_t      level;      // おんりょう
            int         volume;     // おんりょうせってい
            int16_t*    sample;     // さんぷる
            uint32_t    size;       // さいず
            uint32_t    pos;        // いち
            uint32_t    step;       // すてっぷち
            uint32_t    rate;       // さんぷるのれーと
        };

        void        RhythmMix(Sample* buffer, uint32_t count);

    // リズム音源関係
        Rhythm      rhythm[6];
        int8_t      rhythmtl;       // リズム全体の音量
        int         rhythmtvol;
        uint8_t     rhythmkey;      // リズムのキー
    }; // class OPNA

    //  YM2610/B(OPNB) ---------------------------------------------------
    class OPNB : public OPNABase {
    public:
        OPNB();
        virtual ~OPNB();

        bool    Init(uint32_t c, uint32_t r, bool = false,
                     uint8_t *_adpcma = 0, int _adpcma_size = 0,
                     uint8_t *_adpcmb = 0, int _adpcmb_size = 0);

        bool    SetRate(uint32_t c, uint32_t r, bool = false);
        void    Mix(Sample* buffer, int nsamples);

        void        Reset();
        void        SetReg(uint32_t addr, uint32_t data);
        uint32_t    GetReg(uint32_t addr);
        uint32_t    ReadStatusEx();

        void    SetVolumeADPCMATotal(int db);
        void    SetVolumeADPCMA(int index, int db);
        void    SetVolumeADPCMB(int db);

//      void    SetChannelMask(uint32_t mask);

    private:
        struct ADPCMA {
            uint8_t     pan;        // ぱん
            int8_t      level;      // おんりょう
            int         volume;     // おんりょうせってい
            uint32_t    pos;        // いち
            uint32_t    step;       // すてっぷち

            uint32_t    start;      // 開始
            uint32_t    stop;       // 終了
            uint32_t    nibble;     // 次の 4 bit
            int         adpcmx;     // 変換用
            int         adpcmd;     // 変換用
        };

        int         DecodeADPCMASample(uint32_t);
        void        ADPCMAMix(Sample* buffer, uint32_t count);
        static void InitADPCMATable();

    // ADPCMA 関係
        uint8_t*    adpcmabuf;      // ADPCMA ROM
        int         adpcmasize;
        ADPCMA      adpcma[6];
        int8_t      adpcmatl;       // ADPCMA 全体の音量
        int         adpcmatvol;
        uint8_t     adpcmakey;      // ADPCMA のキー
        int         adpcmastep;
        uint8_t     adpcmareg[32];

        static int  jedi_table[(48+1)*16];

        Channel4    ch[6];
    }; // class OPNB

    //  YM2612/3438(OPN2) ----------------------------------------------------
    class OPN2 : public OPNBase {
    public:
        OPN2();
        virtual ~OPN2() {}

        bool        Init(uint32_t c, uint32_t r, bool = false, const char* = 0);
        bool        SetRate(uint32_t c, uint32_t r, bool);

        void        Reset();
        void        Mix(Sample* buffer, int nsamples);
        void        SetReg(uint32_t addr, uint32_t data);
        uint32_t    GetReg(uint32_t addr);
        uint32_t    ReadStatus() { return status & 0x03; }
        uint32_t    ReadStatusEx() { return 0xff; }

        void        SetChannelMask(uint32_t mask);

    private:
        virtual void Intr(bool) {}

        void        SetStatus(uint32_t bit);
        void        ResetStatus(uint32_t bit);

        uint32_t    fnum[3];
        uint32_t    fnum3[3];
        uint8_t     fnum2[6];

    // 線形補間用ワーク
        int32_t     mixc, mixc1;

        Channel4    ch[3];
    }; // class OPN2

    inline void OPNBase::RebuildTimeTable() {
        int p = prescale;
        prescale = -1;
        SetPrescaler(p);
    }

    inline void OPNBase::SetVolumePSG(int db) {
        psg.SetVolume(db);
    }
} // namespace FM

// ---------------------------------------------------------------------------

#endif // FM_OPNA_H
