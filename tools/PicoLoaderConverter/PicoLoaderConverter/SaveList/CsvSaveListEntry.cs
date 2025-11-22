using CsvHelper.Configuration.Attributes;
using PicoLoaderConverter.Csv;

namespace PicoLoaderConverter.SaveList;

sealed class CsvSaveListEntry
{
    [Name("gameCode")]
    public string GameCode { get; set; } = string.Empty;

    [Name("saveType")]
    public string SaveType { get; set; } = string.Empty;

    [Name("saveSize")]
    [TypeConverter(typeof(HexNumberConverter))]
    public int SaveSize { get; set; }
}
