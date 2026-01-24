using Newtonsoft.Json;

namespace PicoLoaderConverter.Json;

sealed class JsonHexBytesConverter : JsonConverter
{
    public override bool CanConvert(Type objectType)
    {
        return objectType == typeof(byte[]);
    }

    public override object? ReadJson(JsonReader reader, Type objectType, object? existingValue, JsonSerializer serializer)
    {
        string? text = reader.Value as string;
        var bytesString = text?.Split(" ", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries) ?? [];
        return bytesString.Select(byteString =>
        {
            if (byteString.StartsWith("0x"))
            {
                byteString = byteString[2..];
            }
            return Convert.ToByte(byteString, 16);
        }).ToArray();
    }

    public override void WriteJson(JsonWriter writer, object? value, JsonSerializer serializer)
    {
        throw new NotImplementedException();
    }
}
