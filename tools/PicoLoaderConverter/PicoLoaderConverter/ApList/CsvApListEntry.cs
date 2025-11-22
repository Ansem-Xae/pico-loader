using CsvHelper.Configuration.Attributes;
using PicoLoaderConverter.Csv;

namespace PicoLoaderConverter.ApList;

sealed class CsvApListEntry
{
    [Name("gameCode")]
    public string GameCode { get; set; } = string.Empty;

    [Name("gameVersion")]
    public int GameVersion { get; set; }

    [Name("dsprotVersion")]
    public string DSProtectVersion { get; set; } = string.Empty;

    [Name("dsprotFuncMask")]
    [TypeConverter(typeof(BinaryNumberConverter))]
    public int DSProtectFunctionMask { get; set; }

    [Name("regularOvlId")]
    public int RegularOverlayId { get; set; }

    [Name("regularOffset")]
    [TypeConverter(typeof(HexNumberConverter))]
    public int RegularOffset { get; set; }

    [Name("sOvlId")]
    public int SOverlayId { get; set; }

    [Name("sOffset")]
    [TypeConverter(typeof(HexNumberConverter))]
    public int SOffset { get; set; }
}
