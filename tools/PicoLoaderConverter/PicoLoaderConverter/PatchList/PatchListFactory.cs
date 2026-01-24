using Newtonsoft.Json;
using PicoLoaderConverter.Common;

namespace PicoLoaderConverter.PatchList;

sealed class PatchListFactory
{
    public PatchList FromJson(string json)
    {
        var result = JsonConvert.DeserializeObject<PatchListEntry[]>(json) ?? [];
        return new PatchList(result);
    }

    public byte[] ToBinary(PatchList patchList)
    {
        var memoryStream = new MemoryStream();
        using var writer = new BinaryWriter(memoryStream);
        var sortedEntries = patchList.Entries
            .OrderBy(entry => GameCodeHelper.GameCodeToUint(entry.GameCode))
            .ThenBy(entry => entry.GameVersion)
            .ToArray();
        writer.Write(sortedEntries.Length);
        foreach (var entry in sortedEntries)
        {
            writer.Write(GameCodeHelper.GameCodeToUint(entry.GameCode));
            writer.Write(0);
        }
        for (int i = 0; i < sortedEntries.Length; i++)
        {
            var entry = sortedEntries[i];
            long entryStart = writer.BaseStream.Position;
            writer.BaseStream.Position = 4 + i * 8 + 4;
            writer.Write((uint)((entryStart << 8) | entry.GameVersion));
            writer.BaseStream.Position = entryStart;
            writer.Write((ushort)0);
            writer.Write((ushort)entry.Patches.Length);
            foreach (var patch in entry.Patches)
            {
                WritePatch(writer, patch);
            }
            long entryEnd = writer.BaseStream.Position;
            writer.BaseStream.Position = entryStart;
            writer.Write((ushort)(entryEnd - entryStart));
            writer.BaseStream.Position = entryEnd;

        }
        return memoryStream.ToArray();
    }

    private void WritePatch(BinaryWriter writer, PatchListEntryPatch patch)
    {
        writer.Write((byte)patch.Type);
        writer.Write((byte)0);
        switch (patch.Type)
        {
            case PatchType.Replace:
            {
                writer.Write((ushort)patch.Data.Length);
                writer.Write(patch.Address);
                writer.Write(patch.Data);
                while ((writer.BaseStream.Position % 4) != 0)
                {
                    writer.Write((byte)0);
                }
                break;
            }
            case PatchType.Metafortress:
            {
                writer.Write((ushort)patch.Addresses.Length);
                foreach (uint address in patch.Addresses)
                {
                    writer.Write(address);
                }
                break;
            }
            default:
            {
                throw new NotImplementedException();
            }
        }
    }
}
