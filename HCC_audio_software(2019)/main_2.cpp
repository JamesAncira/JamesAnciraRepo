/*
	wav_spec.samples(4096) how much of a chunk of a wav file to put into buffer
	audio_buf is filled with a pointer to an allocated buffer containing the audio data
	audio_len is filled with the length of that audio buffer in bytes

	audio_len is 35k

	*audio_buf[audio_len] <-contains audio data

	int array_size_short = audio_len; <-size of short array?

	array_short_data[array_size_short] = audio_buf[audio_len]; <-give audio_buf sample data to array_short_data
	
	array_short_data[]; <-what does this contain
*/