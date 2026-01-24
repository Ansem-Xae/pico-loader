using Newtonsoft.Json;

namespace PicoLoaderConverter.Json;

sealed class JsonHexNumberConverter : JsonConverter
{
    public override bool CanConvert(Type objectType)
    {
        return objectType == typeof(uint);
    }

    public override object? ReadJson(JsonReader reader, Type objectType, object? existingValue, JsonSerializer serializer)
    {
        string? text = reader.Value as string;
        if (text?.StartsWith("0x") is true)
        {
            text = text[2..];
        }
        return Convert.ToUInt32(text, 16);
    }

    public override void WriteJson(JsonWriter writer, object? value, JsonSerializer serializer)
    {
        throw new NotImplementedException();
    }
}
