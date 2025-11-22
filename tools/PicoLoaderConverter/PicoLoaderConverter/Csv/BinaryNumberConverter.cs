using CsvHelper;
using CsvHelper.Configuration;
using CsvHelper.TypeConversion;

namespace PicoLoaderConverter.Csv;

sealed class BinaryNumberConverter : DefaultTypeConverter
{
    public override object? ConvertFromString(string? text, IReaderRow row, MemberMapData memberMapData)
    {
        if (text?.StartsWith("0b") is true)
        {
            text = text[2..];
        }
        return Convert.ToInt32(text, 2);
    }

    public override string? ConvertToString(object? value, IWriterRow row, MemberMapData memberMapData)
    {
        return Convert.ToString(Convert.ToInt32(value), 2);
    }
}
