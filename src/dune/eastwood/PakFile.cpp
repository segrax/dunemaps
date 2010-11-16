#include "stdafx.h"

#include "Exception.h"
#include "Log.h"
#include "PakFile.h"

using namespace eastwood;

PakFile::PakFile(std::istream *stream) : _stream(stream), _fileEntry(std::vector<PakFileEntry>())
{
    readIndex();
}

PakFile::~PakFile()
{
	delete _stream;
}

void PakFile::readIndex()
{
    char name[256];

    uint32_t offset = readU32LE(*_stream);

    while(offset) {
        uint32_t start = offset,
                 size;

		_stream->getline(name, 256, 0);

        LOG_INFO("PakFile", "Found file %s", name);

		offset = readU32LE(*_stream);
        size = (offset != 0) ? offset : getStreamSize(*_stream);

		PakFileEntry file;
		file.startOffset = start;
		file.fileName = name;
		file.endOffset = size - 1;

		std::transform( file.fileName.begin(), file.fileName.end(), file.fileName.begin(), ::tolower );

        _fileEntry.push_back(file);
    }
}

std::istream *PakFile::getFileStream(std::string fileName)
{
    std::vector<uint8_t> buffer;
    PakFileEntry fileEntry = { 0, 0, "" };

	std::transform( fileName.begin(), fileName.end(), fileName.begin(), ::tolower );

    for(std::vector<PakFileEntry>::iterator it = _fileEntry.begin(); it <= _fileEntry.end(); it++ )
    {
        if(it == _fileEntry.end())
            throw(FileNotFoundException(LOG_ERROR, "PakFile", fileName));

        if((fileEntry = *it).fileName == fileName)
            break;
    }

    buffer.resize(fileEntry.endOffset - fileEntry.startOffset + 1);

    if(buffer.size() == 0)
        throw(NullSizeException(LOG_ERROR, "PakFile", fileName));

    _stream->seekg(fileEntry.startOffset, std::ios::beg);
    _stream->read((char*)&buffer.front(), buffer.size());

    return new std::istringstream(std::string((const char*)&buffer.front(), buffer.size()));
}
// vim:ts=8:sw=4:et

std::string *PakFile::getFile(std::string fileName) {
	PakFileEntry fileEntry = { 0, 0, "" };

	std::transform( fileName.begin(), fileName.end(), fileName.begin(), ::tolower );

	for(std::vector<PakFileEntry>::iterator it = _fileEntry.begin(); it <= _fileEntry.end(); it++ )
    {
        if(it == _fileEntry.end())
            throw(FileNotFoundException(LOG_ERROR, "PakFile", fileName));

        if((fileEntry = *it).fileName == fileName)
            break;
    }

	std::string   *tmp = new std::string();
	size_t finalSize = fileEntry.endOffset - fileEntry.startOffset + 1;

	char	*buf = new char[ finalSize + 1] ;

	_stream->seekg(fileEntry.startOffset, std::ios::beg);
	_stream->read( (char*) buf, finalSize );
	tmp->append(buf, finalSize );

	delete[] buf;

	return tmp;
}
