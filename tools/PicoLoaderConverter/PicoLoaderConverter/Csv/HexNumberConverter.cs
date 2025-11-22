using CsvHelper;
using CsvHelper.Configuration;
using CsvHelper.TypeConversion;

namespace PicoLoaderConverter.Csv;

sealed class HexNumberConverter : DefaultTypeConverter
{
    public override object? ConvertFromString(string? text, IReaderRow row, MemberMapData memberMapData)
    {
        if (text?.StartsWith("0x") is true)
        {
            text = text[2..];
        }
        return Convert.ToInt32(text, 16);
    }

    public override string? ConvertToString(object? value, IWriterRow row, MemberMapData memberMapData)
    {
        return "0x" + Convert.ToString(Convert.ToInt32(value), 16);
    }
}
