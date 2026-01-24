using Newtonsoft.Json;
using PicoLoaderConverter.Json;

namespace PicoLoaderConverter.PatchList;

sealed class PatchListEntryPatch
{
    public PatchType Type { get; init; }
    public string Description { get; init; } = string.Empty;

    // replace
    [JsonConverter(typeof(JsonHexNumberConverter))]
    public uint Address { get; init; }

    [JsonConverter(typeof(JsonHexBytesConverter))]
    public byte[] Data { get; init; } = [];

    // metafortress
    [JsonProperty(ItemConverterType = typeof(JsonHexNumberConverter))]
    public uint[] Addresses { get; init; } = [];
}
