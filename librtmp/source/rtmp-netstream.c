#include "rtmp-netstream.h"
#include "amf0.h"
#include <stdlib.h>
#include <string.h>

static const char* s_rtmp_level[] = { "warning", "status", "error" };
static const char* s_rtmp_stream[] = { "live", "record", "append" };

// @param[in] streamName flv:sample, mp3:sample, H.264/AAC: mp4:sample.m4v
// @param[in] start -2-live/vod, -1-live only, >=0-seek position
// @param[in] duration <=-1-all, 0-single frame, >0-period
// @param[in] reset 1-flush any previous playlist, 0-don't flush
uint8_t* rtmp_netstream_play(uint8_t* out, size_t bytes, int transactionId, const char* name, double start, double duration, int reset)
{
	uint8_t* end = out + bytes;
	const char* command = "play";
	
	if (NULL == name)
		return NULL;

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, name, strlen(name)); // Stream Name
	out = AMFWriteDouble(out, end, start); // Start Number
	out = AMFWriteDouble(out, end, duration); // Duration Number
	out = AMFWriteBoolean(out, end, (uint8_t)reset); // Reset Boolean
	return out;
}

// reponse: none
uint8_t* rtmp_netstream_delete_stream(uint8_t* out, size_t bytes, int transactionId, int id)
{
	uint8_t* end = out + bytes;
	const char* command = "deleteStream";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteDouble(out, end, id); // Stream ID
	return out;
}

uint8_t* rtmp_netconnection_close_stream(uint8_t* out, size_t bytes, int transactionId, int streamId)
{
	uint8_t* end = out + bytes;
	const char* command = "closeStream";

	out = AMFWriteString(out, end, command, strlen(command));
	out = AMFWriteDouble(out, end, transactionId);
	out = AMFWriteNull(out, end);
	out = AMFWriteDouble(out, end, streamId);
	return out;
}

// reponse: enable-false: none, enable-true: onStatus with NetStream.Seek.Notify/NetStream.Play.Start
uint8_t* rtmp_netstream_receive_audio(uint8_t* out, size_t bytes, int transactionId, int enable)
{
	uint8_t* end = out + bytes;
	const char* command = "receiveAudio";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteBoolean(out, end, (uint8_t)enable); // Bool Flag
	return out;
}

// reponse: enable-false: none, enable-true: onStatus with NetStream.Seek.Notify/NetStream.Play.Start
uint8_t* rtmp_netstream_receive_video(uint8_t* out, size_t bytes, int transactionId, int enable)
{
	uint8_t* end = out + bytes;
	const char* command = "receiveVideo";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteBoolean(out, end, (uint8_t)enable); // Bool Flag
	return out;
}

// response: onStatus beginning of publish
uint8_t* rtmp_netstream_publish(uint8_t* out, size_t bytes, int transactionId, const char* playpath, enum rtmp_stream_type_t stream)
{
	uint8_t* end = out + bytes;
	const char* command = "publish";
	const char* streamType = s_rtmp_stream[stream % (sizeof(s_rtmp_stream) / sizeof(s_rtmp_stream[0]))];

	if (NULL == playpath)
		return NULL;

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, playpath, strlen(playpath)); // Publishing Name
	out = AMFWriteString(out, end, streamType, strlen(streamType)); // Publishing Type
	return out;
}

// response: success: onStatus-NetStream.Seek.Notify, failure: _error message
uint8_t* rtmp_netstream_seek(uint8_t* out, size_t bytes, int transactionId, double ms)
{
	uint8_t* end = out + bytes;
	const char* command = "seek";
	
	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteDouble(out, end, ms); // milliSeconds Number
	return out;
}

// @param[in] pause 1-pausing, 0-resuing
// response: success: onStatus-NetStream.Pause.Notify/NetStream.Unpause.Notify, failure: _error message
uint8_t* rtmp_netstream_pause(uint8_t* out, size_t bytes, int transactionId, int pause, double ms)
{
	uint8_t* end = out + bytes;
	const char* command = "pause";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteBoolean(out, end, (uint8_t)pause); // pause/unpause
	out = AMFWriteDouble(out, end, ms); // milliSeconds Number
	return out;
}

uint8_t* rtmp_netstream_release_stream(uint8_t* out, size_t bytes, int transactionId, const char* playpath)
{
	uint8_t* end = out + bytes;
	const char* command = "releaseStream";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, playpath, strlen(playpath)); // playpath
	return out;
}

uint8_t* rtmp_netstream_fcpublish(uint8_t* out, size_t bytes, int transactionId, const char* playpath)
{
	uint8_t* end = out + bytes;
	const char* command = "FCPublish";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, playpath, strlen(playpath)); // playpath
	return out;
}

uint8_t* rtmp_netstream_fcunpublish(uint8_t* out, size_t bytes, int transactionId, const char* playpath)
{
	uint8_t* end = out + bytes;
	const char* command = "FCUnpublish";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, playpath, strlen(playpath)); // playpath
	return out;
}

uint8_t* rtmp_netstream_fcsubscribe(uint8_t* out, size_t bytes, int transactionId, const char* subscribepath)
{
	uint8_t* end = out + bytes;
	const char* command = "FCSubscribe";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, subscribepath, strlen(subscribepath)); // subscribepath
	return out;
}

uint8_t* rtmp_netstream_fcunsubscribe(uint8_t* out, size_t bytes, int transactionId, const char* subscribepath)
{
	uint8_t* end = out + bytes;
	const char* command = "FCUnsubscribe";

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object
	out = AMFWriteString(out, end, subscribepath, strlen(subscribepath)); // subscribepath
	return out;
}

uint8_t* rtmp_netstream_onstatus(uint8_t* out, size_t bytes, int transactionId, enum rtmp_level_t level, const char* code, const char* description)
{
	uint8_t* end = out + bytes;
	const char* command = "onStatus";
	const char* slevel = s_rtmp_level[level % (sizeof(s_rtmp_level) / sizeof(s_rtmp_level[0]))];

	if (NULL == code || NULL == description)
		return NULL;

	out = AMFWriteString(out, end, command, strlen(command)); // Command Name
	out = AMFWriteDouble(out, end, transactionId); // Transaction ID
	out = AMFWriteNull(out, end); // command object

	out = AMFWriteObject(out, end);
	out = AMFWriteNamedString(out, end, "level", 5, slevel, strlen(slevel));
	out = AMFWriteNamedString(out, end, "code", 4, code, strlen(code));
	out = AMFWriteNamedString(out, end, "description", 11, description, strlen(description));
	out = AMFWriteObjectEnd(out, end);
	return out;
}
