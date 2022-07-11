
#include "../include/AbstractRecord.h"

AbstractRecord::AbstractRecord(RecordHeader &&h) : header(std::move(h))
{
}
