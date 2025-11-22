using CsvHelper.Configuration;
using CsvHelper;
using System.Globalization;
using System.Numerics;

namespace PicoLoaderConverter.SaveList;

sealed class SaveListFactory
{
    private const int BINARY_SAVE_LIST_ENTRY_SIZE = 8;

    public SaveList FromBinary(byte[] data)
    {
        int entryCount = data.Length / BINARY_SAVE_LIST_ENTRY_SIZE;
        var entries = new SaveListEntry[entryCount];
        using (var reader = new BinaryReader(new MemoryStream(data)))
        {
            for (int i = 0; i < entryCount; i++)
            {
                entries[i] = ReadBinarySaveListEntry(reader);
            }
        }

        return new SaveList(entries);
    }

    public byte[] ToBinary(SaveList saveList)
    {
        var memoryStream = new MemoryStream();
        using var writer = new BinaryWriter(memoryStream);
        foreach (var entry in saveList.Entries)
        {
            WriteBinarySaveListEntry(writer, entry);
        }
        return memoryStream.ToArray();
    }

    public SaveList FromCsv(string csv)
    {
        var config = new CsvConfiguration(CultureInfo.InvariantCulture)
        {
            Delimiter = ";"
        };
        using var reader = new StringReader(csv);
        using var csvReader = new CsvReader(reader, config);
        var csvEntries = csvReader
            .GetRecords<CsvSaveListEntry>()
            .Select(ConvertFromCvsSaveListEntry)
            .OrderBy(entry => entry.GameCode)
            .ToArray();
        return new SaveList(csvEntries);
    }

    public string ToCsv(SaveList saveList)
    {
        var config = new CsvConfiguration(CultureInfo.InvariantCulture)
        {
            Delimiter = ";"
        };
        using var writer = new StringWriter();
        using var csvWriter = new CsvWriter(writer, config);
        csvWriter.WriteRecords(
            saveList.Entries
                .Select(ConvertToCvsSaveListEntry)
                .OrderBy(entry => entry.GameCode));
        return writer.ToString();
    }

    private SaveListEntry ReadBinarySaveListEntry(BinaryReader reader)
    {
        uint gameCode = reader.ReadUInt32();
        var saveType = (CardSaveType)reader.ReadByte();
        byte saveSize = reader.ReadByte();
        reader.ReadUInt16();
        return new SaveListEntry(
            gameCode,
            saveType,
            saveSize);
    }

    public void WriteBinarySaveListEntry(BinaryWriter writer, SaveListEntry entry)
    {
        writer.Write(entry.GameCode);
        writer.Write((byte)entry.SaveType);
        writer.Write(entry.SaveSize);
        writer.Write((ushort)0);
    }

    private SaveListEntry ConvertFromCvsSaveListEntry(CsvSaveListEntry csvSaveListEntry)
    {
        if (csvSaveListEntry.SaveSize < 0 ||
            (csvSaveListEntry.SaveSize > 0 && !BitOperations.IsPow2(csvSaveListEntry.SaveSize)))
        {
            throw new ArgumentException(
                $"Save size 0x{csvSaveListEntry.SaveSize:X} is not supported. It must be a power of two.",
                nameof(csvSaveListEntry));
        }
        
        return new SaveListEntry(
            GameCodeToUint(csvSaveListEntry.GameCode),
            ParseSaveType(csvSaveListEntry.SaveType),
            (byte)(csvSaveListEntry.SaveSize == 0 ? 0 : BitOperations.Log2((uint)csvSaveListEntry.SaveSize)));
    }

    private CsvSaveListEntry ConvertToCvsSaveListEntry(SaveListEntry saveListEntry)
    {
        return new CsvSaveListEntry
        {
            GameCode = $"{(char)(saveListEntry.GameCode & 0xFF)}{(char)(saveListEntry.GameCode >> 8 & 0xFF)}" +
                $"{(char)(saveListEntry.GameCode >> 16 & 0xFF)}{(char)(saveListEntry.GameCode >> 24)}",
            SaveType = FormatSaveType(saveListEntry.SaveType),
            SaveSize = saveListEntry.SaveSize == 0 ? 0 : (1 << saveListEntry.SaveSize)
        };
    }

    private uint GameCodeToUint(string gameCode)
    {
        if (gameCode.Length != 4)
        {
            throw new ArgumentException(
                $"Game code '{gameCode}' is not valid. It must consist of exactly 4 characters.", nameof(gameCode));
        }
        return gameCode[0] | (uint)gameCode[1] << 8 | (uint)gameCode[2] << 16 | (uint)gameCode[3] << 24;
    }

    private CardSaveType ParseSaveType(string saveType)
    {
        return saveType.ToLowerInvariant() switch
        {
            "none" => CardSaveType.None,
            "eeprom" => CardSaveType.Eeprom,
            "flash" => CardSaveType.Flash,
            "nand" => CardSaveType.Nand,
            _ => throw new ArgumentException(
                $"Save type '{saveType}' could not be parsed.", nameof(saveType))
        };
    }

    private string FormatSaveType(CardSaveType saveType)
    {
        return saveType switch
        {
            CardSaveType.None => "none",
            CardSaveType.Eeprom => "eeprom",
            CardSaveType.Flash => "flash",
            CardSaveType.Nand => "nand",
            _ => throw new ArgumentException("Invalid card save type.", nameof(saveType))
        };
    }
}
