// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <numeric>
#include "../../../OptFrame/RandGen.hpp"
#include "../../../OptFrame/Util/RandGenMersenneTwister.hpp"

#include <string>
#include <fstream>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

using namespace std;
using namespace optframe;
using namespace EFP;
extern int nThreads;

typedef struct WAV_HEADER
{
	/* RIFF Chunk Descriptor */
	uint8_t RIFF[4];        // RIFF Header Magic header
	uint32_t ChunkSize;      // RIFF Chunk Size
	uint8_t WAVE[4];        // WAVE Header
	/* "fmt" sub-chunk */
	uint8_t fmt[4];         // FMT header
	uint32_t Subchunk1Size;  // Size of the fmt chunk
	uint16_t AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t NumOfChan;      // Number of channels 1=Mono 2=Sterio
	uint32_t SamplesPerSec;  // Sampling Frequency in Hz
	uint32_t bytesPerSec;    // bytes per second
	uint16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
	uint16_t bitsPerSample;  // Number of bits per sample
	/* "data" sub-chunk */
	uint8_t Subchunk2ID[4]; // "data"  string
	uint32_t Subchunk2Size;  // Sampled data length
} wav_hdr;

// Function prototypes
int getFileSize(FILE* inFile);

// WAVE PCM soundfile format (you can find more in https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ )
//typedef struct header_file
//{
//    char chunk_id[4];
//    int chunk_size;
//    char format[4];
//    char subchunk1_id[4];
//    int subchunk1_size;
//    short int audio_format;
//    short int num_channels;
//    int sample_rate;			// sample_rate denotes the sampling rate.
//    int byte_rate;
//    short int block_align;
//    short int bits_per_sample;
//    char subchunk2_id[4];
//    int subchunk2_size;			// subchunk2_size denotes the number of samples.
//} header;
//
//typedef struct header_file* header_p;



int readWAV(int argc, char **argv)
{

//	FILE * infile = fopen("./completed.wav","rb");		// Open wave file in read mode
//	FILE * outfile = fopen("Output.txt","wb");		// Create output ( wave format) file in write mode;
//
//
//	int BUFSIZE = 256;					// BUFSIZE can be changed according to the frame size required (eg:512)
//	int count = 0;						// For counting number of frames in wave file.
//	short int buff16[BUFSIZE];				// short int used for 16 bit as input data format is 16 bit PCM audio
//	header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
//	int nb;							// variable storing number of bytes returned
//	if (infile)
//	{
//		fread(meta, 1, sizeof(header), infile);
//		fwrite(meta,1, sizeof(*meta), outfile);
//		cout << " Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
//		cout << " Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
//		cout << " Number of samples in wave file are " << meta->subchunk2_size << " samples" << endl;
//		cout << " The number of channels of the file is "<< meta->num_channels << " channels" << endl;
//
//
//
//		while (!feof(infile))
//		{
//			nb = fread(buff16,1,BUFSIZE,infile);		// Reading data in chunks of BUFSIZE
//			//cout << nb <<endl;
//			count++;					// Incrementing Number of frames
//
//
//
//			fwrite(buff16,1,nb,outfile);			// Writing read data into output file (.txt)
//			cout<<nb<<"\t"<<buff16<<endl;
//			getchar();
//		}
//	cout << " Number of frames in the input wave file are " <<count << endl;
//	}
//
//
//return 0;

	cout<<"welcome to WAVRead project"<<endl;

	 wav_hdr wavHeader;
	    int headerSize = sizeof(wav_hdr), filelength = 0;

	    const char* filePath;

	        filePath = "./completed.wav";
	        cout << "Input wave file name: " << filePath << endl;


	    FILE* wavFile = fopen(filePath, "r");
	    if (wavFile == nullptr)
	    {
	        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
	        return 1;
	    }

	    //Read the header
	    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
	    cout << "Header Read " << bytesRead << " bytes." << endl;
	    if (bytesRead > 0)
	    {
	        //Read the data
//	        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
	        //TODO check the use
//	        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
	        static const uint16_t BUFFER_SIZE = 4096;
	        int8_t* buffer = new int8_t[BUFFER_SIZE];
	        while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
	        {
	            /** DO SOMETHING WITH THE WAVE DATA HERE **/
	            cout << "Read " << bytesRead << " bytes." << endl;

	            cout << *buffer <<endl;
	            getchar();
	        }
	        delete [] buffer;
	        buffer = nullptr;
	        filelength = getFileSize(wavFile);

	        cout << "File is                    :" << filelength << " bytes." << endl;
	        cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
	        cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
	        cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
	        cout << "Data size                  :" << wavHeader.ChunkSize << endl;

	        // Display the sampling Rate from the header
	        cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
	        cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
	        cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
	        cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
	        cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
	        cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
	        // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

	        cout << "Block align                :" << wavHeader.blockAlign << endl;
	        cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
	    }
	    fclose(wavFile);
	    return 0;
}

// find the file size
int getFileSize(FILE* inFile)
{
	int fileSize = 0;
	fseek(inFile, 0, SEEK_END);

	fileSize = ftell(inFile);

	fseek(inFile, 0, SEEK_SET);
	return fileSize;
}

